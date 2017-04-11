/*
 * Copyright 2016 Freeman Zhang <zhanggyb@gmail.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <skland/gui/window.hpp>

#include <skland/gui/application.hpp>
#include <skland/gui/abstract-view.hpp>
#include <skland/gui/mouse-event.hpp>
#include <skland/gui/key-event.hpp>
#include <skland/gui/context.hpp>
#include <skland/gui/abstract-shell-frame.hpp>
#include <skland/gui/toplevel-shell-surface.hpp>
#include <skland/gui/sub-surface.hpp>
#include <skland/gui/title-bar.hpp>

#include <skland/gui/shared-memory-pool.hpp>
#include <skland/gui/buffer.hpp>

#include <skland/stock/theme.hpp>

#include "internal/display-registry.hpp"
#include "internal/abstract-shell-view-iterators.hpp"
#include "internal/abstract-view-iterators.hpp"
#include "internal/abstract-event-handler-mouse-task-iterator.hpp"

#include <skland/graphic/canvas.hpp>
#include <skland/graphic/paint.hpp>
#include <skland/graphic/path.hpp>

#include "SkCanvas.h"

namespace skland {

struct Window::Private {

  Private(const Private &) = delete;
  Private &operator=(const Private &) = delete;

  Private()
      : main_surface(nullptr),
        title_bar(nullptr),
        content_view(nullptr) {}

  ~Private() {}

  Surface *main_surface;

  /* Properties for frame surface, JUST experimental */
  SharedMemoryPool pool;

  Buffer frame_buffer;
  std::shared_ptr<Canvas> frame_canvas;

  /* Properties for main surface, JUST experimental */
  Buffer main_buffer;
  std::shared_ptr<Canvas> main_canvas;

  TitleBar *title_bar;
  AbstractView *content_view;

};

const Margin Window::kResizingMargin(5, 5, 5, 5);

Window::Window(const char *title)
    : Window(640, 480, title) {
}

Window::Window(int width, int height, const char *title)
    : AbstractShellView(width, height, title, nullptr) {
  p_.reset(new Private);

  // Create a sub surface for views:
  Surface *parent = GetShellSurface();
  p_->main_surface = SubSurface::Create(parent, this, Theme::shadow_margin());
  DBG_ASSERT(p_->main_surface->parent() == parent);
  DBG_ASSERT(p_->main_surface->below() == parent);
  wayland::Region empty_region;
  empty_region.Setup(Display::Registry().wl_compositor());
  p_->main_surface->SetInputRegion(empty_region);

  // Create the default title bar:
  TitleBar* titlebar = new TitleBar;
  p_->title_bar = titlebar;
  AttachView(p_->title_bar);

  titlebar->SetTitle(title);
  titlebar->Resize(GetWidth(), 22);

  AbstractButton *button = titlebar->GetButton(TitleBar::kButtonClose);
  button->clicked().Connect(this, static_cast<void (Window::*)(SLOT)>(&AbstractShellView::Close));

  button = titlebar->GetButton(TitleBar::kButtonMaximize);
  button->clicked().Connect(this, static_cast<void (Window::*)(SLOT)>(&AbstractShellView::ToggleMaximize));

  button = titlebar->GetButton(TitleBar::kButtonMinimize);
  button->clicked().Connect(this, static_cast<void (Window::*)(SLOT)>(&AbstractShellView::Minimize));

  button = titlebar->GetButton(TitleBar::kButtonFullscreen);
  button->clicked().Connect(this, static_cast<void (Window::*)(SLOT)>(&AbstractShellView::ToggleFullscreen));
}

Window::~Window() {
  if (p_->content_view) p_->content_view->Destroy();
  if (p_->title_bar) p_->title_bar->Destroy();

  delete p_->main_surface;
}

AbstractView *Window::GetTitleBar() const {
  return p_->title_bar;
}

AbstractView *Window::GetContentView() const {
  return p_->content_view;
}

void Window::SetContentView(AbstractView *view) {
  if (p_->content_view == view) return;

  if (p_->content_view) p_->content_view->Destroy();
  // p_->content_view will be set to nullptr in OnViewDetached()

  p_->content_view = view;
  if (nullptr == p_->content_view) return;

  AttachView(p_->content_view);

  Rect geometry = GetContentGeometry();
  p_->content_view->MoveTo(static_cast<int>(geometry.x()), static_cast<int>(geometry.y()));
  p_->content_view->Resize(static_cast<int>(geometry.width()), static_cast<int>(geometry.height()));
}

void Window::OnShown() {
  Surface *shell_surface = this->GetShellSurface();

  // Create buffer:
  int width = GetWidth();
  int height = GetHeight();
  width += shell_surface->margin().lr();
  height += shell_surface->margin().tb();

  int32_t pool_size = width * 4 * height;
  if (p_->main_surface) pool_size *= 2; // double buffer for 2 surfaces

  p_->pool.Setup(pool_size);

  p_->frame_buffer.Setup(p_->pool, width, height,
                          width * 4, WL_SHM_FORMAT_ARGB8888);
  shell_surface->Attach(&p_->frame_buffer);
  p_->frame_canvas.reset(new Canvas((unsigned char *) p_->frame_buffer.GetData(),
                                     p_->frame_buffer.GetSize().width,
                                     p_->frame_buffer.GetSize().height));
  p_->frame_canvas->SetOrigin((float) shell_surface->margin().left,
                               (float) shell_surface->margin().top);
  p_->frame_canvas->Clear();

  if (p_->main_surface) {
    p_->main_buffer.Setup(p_->pool, width, height,
                           width * 4, WL_SHM_FORMAT_ARGB8888,
                           width * 4 * height);
    p_->main_surface->Attach(&p_->main_buffer);
    p_->main_canvas.reset(new Canvas((unsigned char *) p_->main_buffer.GetData(),
                                      p_->main_buffer.GetSize().width,
                                      p_->main_buffer.GetSize().height));
    p_->main_canvas->SetOrigin((float) p_->main_surface->margin().left,
                                (float) p_->main_surface->margin().top);
    p_->main_canvas->Clear();
  }

  RecursiveUpdate();
}

void Window::OnUpdate(AbstractView *view) {
  if (!IsShown()) return;

  Surface *surface = nullptr;

  if (nullptr == view) {
    surface = this->GetShellSurface();
    Iterator it(this);
    PushToTail(&it.redraw_task());
    it.redraw_task().context = Context(surface, p_->frame_canvas);
    DBG_ASSERT(p_->frame_canvas);
    Damage(this,
           0, 0,
           GetWidth() + surface->margin().lr(),
           GetHeight() + surface->margin().tb());
    surface->Commit();
  } else {
    std::shared_ptr<Canvas> canvas;
    if (p_->main_surface) {
      surface = p_->main_surface;
      canvas = p_->main_canvas;
    } else {
      surface = this->GetShellSurface();
      canvas = p_->frame_canvas;
    }

    AbstractView::Iterator it(view);
    PushToTail(&it.redraw_task());
    it.redraw_task().context = Context(surface, canvas);
    DBG_ASSERT(canvas);
    Damage(view,
           view->GetX() + surface->margin().left,
           view->GetY() + surface->margin().top,
           view->GetWidth(),
           view->GetHeight());
    surface->Commit();
  }
}

Surface *Window::GetSurface(const AbstractView *view) const {
  if (nullptr == view)
    return GetShellSurface();

  return nullptr != p_->main_surface ? p_->main_surface : GetShellSurface();
}

void Window::OnResize(const Size &old_size, const Size &new_size) {
  int width = new_size.width;
  int height = new_size.height;

  RectI input_rect(width, height);
  Surface *shell_surface = this->GetShellSurface();

  input_rect.left = shell_surface->margin().left - AbstractShellFrame::kResizingMargin.left;
  input_rect.top = shell_surface->margin().top - AbstractShellFrame::kResizingMargin.top;
  input_rect.Resize(width + AbstractShellFrame::kResizingMargin.lr(),
                    height + AbstractShellFrame::kResizingMargin.tb());

  wayland::Region input_region;
  input_region.Setup(Display::Registry().wl_compositor());
  input_region.Add(input_rect.x(), input_rect.y(),
                   input_rect.width(), input_rect.height());
  shell_surface->SetInputRegion(input_region);

  // Reset buffer:
  width += shell_surface->margin().lr();
  height += shell_surface->margin().tb();

  int pool_size = width * 4 * height;
  if (p_->main_surface) pool_size *= 2;

  p_->pool.Setup(pool_size);

  p_->frame_buffer.Setup(p_->pool, width, height, width * 4, WL_SHM_FORMAT_ARGB8888);
  shell_surface->Attach(&p_->frame_buffer);
  p_->frame_canvas.reset(new Canvas((unsigned char *) p_->frame_buffer.GetData(),
                                     p_->frame_buffer.GetSize().width,
                                     p_->frame_buffer.GetSize().height));
  p_->frame_canvas->SetOrigin(shell_surface->margin().left, shell_surface->margin().top);
  p_->frame_canvas->Clear();

  if (p_->main_surface) {
    p_->main_buffer.Setup(p_->pool, width, height, width * 4, WL_SHM_FORMAT_ARGB8888, width * 4 * height);
    p_->main_surface->Attach(&p_->main_buffer);
    p_->main_canvas.reset(new Canvas((unsigned char *) p_->main_buffer.GetData(),
                                      p_->main_buffer.GetSize().width,
                                      p_->main_buffer.GetSize().height));
    p_->main_canvas->SetOrigin(p_->main_surface->margin().left,
                                p_->main_surface->margin().top);
    p_->main_canvas->Clear();
  }

  if (p_->title_bar) p_->title_bar->Resize(new_size.width, 22);
  if (p_->content_view) {
    Rect geometry = GetContentGeometry();
    p_->content_view->MoveTo(static_cast<int>(geometry.x()), static_cast<int>(geometry.y()));
    p_->content_view->Resize(static_cast<int>(geometry.width()), static_cast<int>(geometry.height()));
  }

  RecursiveUpdate();
}

void Window::OnMouseEnter(MouseEvent *event) {
  AbstractView *view = nullptr;
  int location = GetMouseLocation(event);
  switch (location) {
    case kResizeTop: {
      event->SetCursor(Display::cursor(kCursorTop));
      break;
    }
    case kResizeBottom: {
      event->SetCursor(Display::cursor(kCursorBottom));
      break;
    }
    case kResizeLeft: {
      event->SetCursor(Display::cursor(kCursorLeft));
      break;
    }
    case kResizeRight: {
      event->SetCursor(Display::cursor(kCursorRight));
      break;
    }
    case kResizeTopLeft: {
      event->SetCursor(Display::cursor(kCursorTopLeft));
      break;
    }
    case kResizeTopRight: {
      event->SetCursor(Display::cursor(kCursorTopRight));
      break;
    }
    case kResizeBottomLeft: {
      event->SetCursor(Display::cursor(kCursorBottomLeft));
      break;
    }
    case kResizeBottomRight: {
      event->SetCursor(Display::cursor(kCursorBottomRight));
      break;
    }
    case kTitleBar: {
      view = p_->title_bar;
      break;
    }
    case kClientArea: {
      view = p_->content_view;
      break;
    }
    default: {
      event->SetCursor(Display::cursor(kCursorLeftPtr));
      break;
    }
  }

  if (view) DispatchMouseEnterEvent(view, event);
}

void Window::OnMouseLeave() {
  DispatchMouseLeaveEvent();
}

void Window::OnMouseMove(MouseEvent *event) {
  AbstractView *view = nullptr;
  switch (GetMouseLocation(event)) {
    case kResizeTop: {
      DispatchMouseLeaveEvent();
      event->SetCursor(Display::cursor(kCursorTop));
      break;
    }
    case kResizeBottom: {
      DispatchMouseLeaveEvent();
      event->SetCursor(Display::cursor(kCursorBottom));
      break;
    }
    case kResizeLeft: {
      DispatchMouseLeaveEvent();
      event->SetCursor(Display::cursor(kCursorLeft));
      break;
    }
    case kResizeRight: {
      DispatchMouseLeaveEvent();
      event->SetCursor(Display::cursor(kCursorRight));
      break;
    }
    case kResizeTopLeft: {
      DispatchMouseLeaveEvent();
      event->SetCursor(Display::cursor(kCursorTopLeft));
      break;
    }
    case kResizeTopRight: {
      DispatchMouseLeaveEvent();
      event->SetCursor(Display::cursor(kCursorTopRight));
      break;
    }
    case kResizeBottomLeft: {
      DispatchMouseLeaveEvent();
      event->SetCursor(Display::cursor(kCursorBottomLeft));
      break;
    }
    case kResizeBottomRight: {
      DispatchMouseLeaveEvent();
      event->SetCursor(Display::cursor(kCursorBottomRight));
      break;
    }
    case kTitleBar: {
      event->SetCursor(Display::cursor(kCursorLeftPtr));
      view = p_->title_bar;
      break;
    }
    case kClientArea: {
      event->SetCursor(Display::cursor(kCursorLeftPtr));
      view = p_->content_view;
      break;
    }
    default: {
      event->SetCursor(Display::cursor(kCursorLeftPtr));
      break;
    }
  }

  if (view) DispatchMouseEnterEvent(view, event);

  // Now dispatch mouse move event:
//      task = static_cast<ViewTask *>(handler->p_->mouse_motion_task.next());
//      p_->mouse_event->response_ = InputEvent::kUnknown;
//      while (task) {
//        task->view->OnMouseMove(p_->mouse_event);
//        if (!p_->mouse_event->IsAccepted()) break;
//        task = static_cast<ViewTask *>(task->next());
//      }
}

void Window::OnMouseButton(MouseEvent *event) {
  if ((event->GetButton() == MouseButton::kLeft) &&
      (event->GetState() == MouseButtonState::kPressed)) {

    int location = GetMouseLocation(event);

    if (location == kTitleBar && (nullptr == MouseTaskIterator(this).next())) {
      MoveWithMouse(event);
      event->Ignore();
      return;
    }

    if (location < kResizeMask) {
      ResizeWithMouse(event, (uint32_t) location);
      event->Ignore();
      return;
    }
  }

  DispatchMouseButtonEvent(event);
}

void Window::OnKeyboardKey(KeyEvent *event) {
  if (event->key() == kKey_ESC) {
    Application::Exit();
  }
  event->Accept();
}

void Window::OnDraw(const Context *context) {
  std::shared_ptr<Canvas> canvas = context->canvas();
  canvas->Clear();

  Path path;
  Rect geometry = Rect::FromXYWH(0.f, 0.f, GetWidth(), GetHeight());

  if ((!IsMaximized()) || (!IsFullscreen())) {
    // Drop shadow:
    float radii[] = {
        7.f, 7.f, // top-left
        7.f, 7.f, // top-right
        4.f, 4.f, // bottom-right
        4.f, 4.f  // bottom-left
    };
    path.AddRoundRect(geometry, radii);
    canvas->Save();
    canvas->ClipPath(path, kClipDifference, true);
    DrawShadow(canvas.get());
    canvas->Restore();
  } else {
    path.AddRect(geometry);
  }

  // Fill color:
  Paint paint;
  paint.SetAntiAlias(true);
  paint.SetColor(0xEFF0F0F0);
  canvas->DrawPath(path, paint);

  // Draw the client area:
  paint.SetColor(0xEFE0E0E0);
  canvas->Save();
  canvas->ClipPath(path, kClipIntersect, true);
  canvas->DrawRect(GetContentGeometry(), paint);
  canvas->Restore();

  canvas->Flush();
}

void Window::OnFocus(bool) {
  OnUpdate(nullptr);
}

void Window::RecursiveUpdate() {
  OnUpdate(nullptr);

  if (p_->title_bar) AbstractShellView::RecursiveUpdate(p_->title_bar);
  if (p_->content_view) AbstractShellView::RecursiveUpdate(p_->content_view);
}

void Window::OnViewAttached(AbstractView */*view*/) {
  // Finalize this virtual method
}

void Window::OnViewDetached(AbstractView *view) {
  if (view == p_->title_bar) {
    p_->title_bar = nullptr;
    return;
  }

  if (view == p_->content_view) {
    p_->content_view = nullptr;
    return;
  }
}

int Window::GetMouseLocation(const MouseEvent *event) const {
  int vlocation, hlocation, location;
  int x = static_cast<int>(event->GetSurfaceXY().x),
      y = static_cast<int>(event->GetSurfaceXY().y);

  // TODO: maximized or frameless

  if (x < (Theme::shadow_margin().left - kResizingMargin.left))
    hlocation = AbstractShellView::kExterior;
  else if (x < Theme::shadow_margin().left + kResizingMargin.left)
    hlocation = AbstractShellView::kResizeLeft;
  else if (x < Theme::shadow_margin().left + GetWidth() - kResizingMargin.right)
    hlocation = AbstractShellView::kInterior;
  else if (x < Theme::shadow_margin().left + GetWidth() + kResizingMargin.right)
    hlocation = AbstractShellView::kResizeRight;
  else
    hlocation = AbstractShellView::kExterior;

  if (y < (Theme::shadow_margin().top - kResizingMargin.top))
    vlocation = AbstractShellView::kExterior;
  else if (y < Theme::shadow_margin().top + kResizingMargin.top)
    vlocation = AbstractShellView::kResizeTop;
  else if (y < Theme::shadow_margin().top + GetHeight() - kResizingMargin.bottom)
    vlocation = AbstractShellView::kInterior;
  else if (y < Theme::shadow_margin().top + GetHeight() + kResizingMargin.bottom)
    vlocation = AbstractShellView::kResizeBottom;
  else
    vlocation = AbstractShellView::kExterior;

  location = vlocation | hlocation;
  if (location & AbstractShellView::kExterior)
    location = AbstractShellView::kExterior;

  if (location == AbstractShellView::kInterior &&
      y < Theme::shadow_margin().top + 22 /* title_bar_size_ */)
    location = AbstractShellView::kTitleBar;
  else if (location == AbstractShellView::kInterior)
    location = AbstractShellView::kClientArea;

  return location;
}

void Window::DrawShadow(Canvas *canvas) {
  float rad = Theme::shadow_radius() - 1.f; // The spread radius
  float offset_x = Theme::shadow_offset_x();
  float offset_y = Theme::shadow_offset_y();

  if (!IsFocused()) {
    rad = (int) rad / 3;
    offset_x = (int) offset_x / 3;
    offset_y = (int) offset_y / 3;
  }

  // shadow map
  SkCanvas *c = canvas->GetSkCanvas();
  sk_sp<SkImage> image = SkImage::MakeFromRaster(*Theme::shadow_pixmap(), nullptr, nullptr);

  // top-left
  c->drawImageRect(image,
                   SkRect::MakeLTRB(0, 0,
                                    2 * Theme::shadow_radius(), 2 * Theme::shadow_radius()),
                   SkRect::MakeXYWH(-rad + offset_x, -rad + offset_y,
                                    2 * rad, 2 * rad),
                   nullptr);

  // top
  c->drawImageRect(image,
                   SkRect::MakeLTRB(2 * Theme::shadow_radius(), 0,
                                    250 - 2 * Theme::shadow_radius(), 2 * Theme::shadow_radius()),
                   SkRect::MakeXYWH(rad + offset_x, -rad + offset_y,
                                    GetWidth() - 2 * rad, 2 * rad),
                   nullptr);

  // top-right
  c->drawImageRect(image,
                   SkRect::MakeLTRB(250 - 2 * Theme::shadow_radius(), 0,
                                    250, 2 * Theme::shadow_radius()),
                   SkRect::MakeXYWH(GetWidth() - rad + offset_x, -rad + offset_y,
                                    2 * rad, 2 * rad),
                   nullptr);

  // left
  c->drawImageRect(image,
                   SkRect::MakeLTRB(0, 2 * Theme::shadow_radius(),
                                    2 * Theme::shadow_radius(), 250 - 2 * Theme::shadow_radius()),
                   SkRect::MakeXYWH(-rad + offset_x, rad + offset_y,
                                    2 * rad, GetHeight() - 2 * rad),
                   nullptr);

  // bottom-left
  c->drawImageRect(image,
                   SkRect::MakeLTRB(0, 250 - 2 * Theme::shadow_radius(),
                                    2 * Theme::shadow_radius(), 250),
                   SkRect::MakeXYWH(-rad + offset_x, GetHeight() - rad + offset_y,
                                    2 * rad, 2 * rad),
                   nullptr);

  // bottom
  c->drawImageRect(image,
                   SkRect::MakeLTRB(2 * Theme::shadow_radius(), 250 - 2 * Theme::shadow_radius(),
                                    250 - 2 * Theme::shadow_radius(), 250),
                   SkRect::MakeXYWH(rad + offset_x, GetHeight() - rad + offset_y,
                                    GetWidth() - 2 * rad, 2 * rad),
                   nullptr);

  // bottom-right
  c->drawImageRect(image,
                   SkRect::MakeLTRB(250 - 2 * Theme::shadow_radius(), 250 - 2 * Theme::shadow_radius(),
                                    250, 250),
                   SkRect::MakeXYWH(GetWidth() - rad + offset_x,
                                    GetHeight() - rad + offset_y,
                                    2 * rad,
                                    2 * rad),
                   nullptr);

  // right
  c->drawImageRect(image,
                   SkRect::MakeLTRB(250 - 2 * Theme::shadow_radius(), 2 * Theme::shadow_radius(),
                                    250, 250 - 2 * Theme::shadow_radius()),
                   SkRect::MakeXYWH(GetWidth() - rad + offset_x, rad + offset_y,
                                    2 * rad, GetHeight() - 2 * rad),
                   nullptr);
}

Rect Window::GetContentGeometry() const {
  int x = 0;
  int y = 0;
  int w = GetWidth();
  int h = GetHeight();
  if (p_->title_bar) {
    y += p_->title_bar->GetHeight();
    h -= p_->title_bar->GetHeight();
  }
  return Rect::FromXYWH(x, y, w, h);
}

}
