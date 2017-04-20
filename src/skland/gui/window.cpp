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

#include <skland/core/assert.hpp>

#include <skland/gui/application.hpp>
#include <skland/gui/abstract-view.hpp>
#include <skland/gui/mouse-event.hpp>
#include <skland/gui/key-event.hpp>
#include <skland/gui/context.hpp>
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
#include <skland/graphic/gradient-shader.hpp>

namespace skland {

struct Window::Private {

  Private(const Private &) = delete;
  Private &operator=(const Private &) = delete;

  Private()
      : main_surface(nullptr),
        title_bar(nullptr),
        content_view(nullptr),
        flags(0) {}

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

  int flags;
};

Window::Window(const char *title, int flags)
    : Window(640, 480, title, flags) {
}

Window::Window(int width, int height, const char *title, int flags)
    : AbstractShellView(width, height, title, nullptr) {
  p_.reset(new Private);
  p_->flags = flags;

  // Create a sub surface for views:
  if (!(flags & kFlagMaskFrameless)) {
    Surface *parent = GetShellSurface();
    p_->main_surface = Surface::Sub::Create(parent, this, Theme::GetShadowMargin());
    _ASSERT(p_->main_surface->parent() == parent);
    _ASSERT(p_->main_surface->below() == parent);
    wayland::Region empty_region;
    empty_region.Setup(Display::Registry().wl_compositor());
    p_->main_surface->SetInputRegion(empty_region);

    // Create the default title bar:
    TitleBar *titlebar = new TitleBar;
    p_->title_bar = titlebar;
    AttachView(p_->title_bar);

    titlebar->SetTitle(title);
    titlebar->Resize(GetWidth(), TitleBar::kHeight);

    AbstractButton *button = titlebar->GetButton(TitleBar::kButtonClose);
    button->clicked().Connect(this, static_cast<void (Window::*)(SLOT)>(&AbstractShellView::Close));

    button = titlebar->GetButton(TitleBar::kButtonMaximize);
    button->clicked().Connect(this, static_cast<void (Window::*)(SLOT)>(&AbstractShellView::ToggleMaximize));

    button = titlebar->GetButton(TitleBar::kButtonMinimize);
    button->clicked().Connect(this, static_cast<void (Window::*)(SLOT)>(&AbstractShellView::Minimize));

    button = titlebar->GetButton(TitleBar::kButtonFullscreen);
    button->clicked().Connect(this, static_cast<void (Window::*)(SLOT)>(&AbstractShellView::ToggleFullscreen));
  }
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

  OnUpdate(nullptr);
  if (p_->title_bar) AbstractShellView::RecursiveUpdate(p_->title_bar);
  if (p_->content_view) AbstractShellView::RecursiveUpdate(p_->content_view);
}

void Window::OnUpdate(AbstractView *view) {
  if (!IsShown()) return;

  Surface *surface = nullptr;

  if (nullptr == view) {
    surface = this->GetShellSurface();
    Iterator it(this);
    PushToTail(&it.redraw_task());
    it.redraw_task().context = Context(surface, p_->frame_canvas);
    _ASSERT(p_->frame_canvas);
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
    _ASSERT(canvas);
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

void Window::OnSizeChange(const Size &old_size, const Size &new_size) {
  int width = new_size.width;
  int height = new_size.height;

  RectI input_rect(width, height);
  Surface *shell_surface = this->GetShellSurface();

  input_rect.left = shell_surface->margin().left - kResizingMargin.left;
  input_rect.top = shell_surface->margin().top - kResizingMargin.top;
  input_rect.Resize(width + kResizingMargin.lr(),
                    height + kResizingMargin.tb());

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

  OnUpdate(nullptr);

  if (p_->title_bar) {
    p_->title_bar->Resize(new_size.width, TitleBar::kHeight);
    AbstractShellView::RecursiveUpdate(p_->title_bar);
  }

  if (p_->content_view) {
    Rect geometry = GetContentGeometry();
    p_->content_view->MoveTo(static_cast<int>(geometry.x()), static_cast<int>(geometry.y()));
    p_->content_view->Resize(static_cast<int>(geometry.width()), static_cast<int>(geometry.height()));
    AbstractShellView::RecursiveUpdate(p_->content_view);
  }
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

void Window::OnMouseDown(MouseEvent *event) {
  if (event->GetButton() == MouseButton::kMouseButtonLeft) {

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

  DispatchMouseDownEvent(event);
}

void Window::OnMouseUp(MouseEvent *event) {
  DispatchMouseUpEvent(event);
}

void Window::OnKeyDown(KeyEvent *event) {
  if (event->key() == kKey_ESC) {
    Application::Exit();
  }
  event->Accept();
}

void Window::OnDraw(const Context *context) {
  if (p_->flags & kFlagMaskFrameless) return;

  Canvas *canvas = context->canvas();
  canvas->Clear();

  Path path;
  Rect geometry = Rect::FromXYWH(0.f, 0.f, GetWidth(), GetHeight());
  bool drop_shadow = !(IsMaximized() || IsFullscreen());
  const Theme::Schema &window_schema = Theme::GetData().window;
  const Theme::Schema &title_bar_schema = Theme::GetData().title_bar;
  Shader shader;
  Point2F points[2];
  points[0].x = 0.f;
  points[0].y = 0.f;
  points[1].x = 0.f;
  points[1].y = GetHeight();

  if (drop_shadow) {
    geometry = geometry.Shrink(0.5f);
    float radii[] = {
        7.f, 7.f, // top-left
        7.f, 7.f, // top-right
        4.f, 4.f, // bottom-right
        4.f, 4.f  // bottom-left
    };
    path.AddRoundRect(geometry, radii);
    Canvas::ClipGuard guard(canvas, path, kClipDifference, true);
    DropShadow(context);
  } else {
    path.AddRect(geometry);
  }

  // Fill color:
  Paint paint;
  paint.SetAntiAlias(true);
  if (window_schema.active.background.shaded) {
    shader = GradientShader::MakeLinear(points,
                                        window_schema.active.background.shaded_colors.data(),
                                        window_schema.active.background.shaded_positions.data(),
                                        window_schema.active.background.shaded_count,
                                        Shader::TileMode::kTileModeClamp);
    paint.SetShader(shader);
  } else {
    paint.SetColor(window_schema.active.background.color);
  }
  canvas->DrawPath(path, paint);
  paint.SetShader(Shader());  // Clear shader

  // Draw outline
  if (drop_shadow) {
    paint.SetColor(window_schema.inactive.outline.color);
    paint.SetStyle(Paint::Style::kStyleStroke);
    canvas->DrawPath(path, paint);
  }

  // Draw the client area:
  Canvas::ClipGuard guard(canvas, path, kClipIntersect, true);

  paint.SetStyle(Paint::Style::kStyleFill);
  if (p_->title_bar) {
    if (title_bar_schema.active.background.shaded) {
      points[1].y = TitleBar::kHeight;
      shader = GradientShader::MakeLinear(points,
                                          title_bar_schema.active.background.shaded_colors.data(),
                                          title_bar_schema.active.background.shaded_positions.data(),
                                          title_bar_schema.active.background.shaded_count,
                                          Shader::TileMode::kTileModeClamp);
      paint.SetShader(shader);
    } else {
      paint.SetColor(title_bar_schema.active.background.color);
    }
    canvas->DrawRect(p_->title_bar->GetGeometry(), paint);
    paint.SetShader(Shader());  // Clear shader
  }

  if (window_schema.active.foreground.shaded) {
    points[1].y = GetHeight();
    shader = GradientShader::MakeLinear(points,
                                        window_schema.active.foreground.shaded_colors.data(),
                                        window_schema.active.foreground.shaded_positions.data(),
                                        window_schema.active.foreground.shaded_count,
                                        Shader::TileMode::kTileModeClamp);
  } else {
    paint.SetColor(window_schema.active.foreground.color);
  }
  canvas->DrawRect(GetContentGeometry(), paint);
//  canvas->Flush();
}

void Window::OnFocus(bool) {
  OnUpdate(nullptr);
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

  if (x < (Theme::GetShadowMargin().left - kResizingMargin.left))
    hlocation = AbstractShellView::kExterior;
  else if (x < Theme::GetShadowMargin().left + kResizingMargin.left)
    hlocation = AbstractShellView::kResizeLeft;
  else if (x < Theme::GetShadowMargin().left + GetWidth() - kResizingMargin.right)
    hlocation = AbstractShellView::kInterior;
  else if (x < Theme::GetShadowMargin().left + GetWidth() + kResizingMargin.right)
    hlocation = AbstractShellView::kResizeRight;
  else
    hlocation = AbstractShellView::kExterior;

  if (y < (Theme::GetShadowMargin().top - kResizingMargin.top))
    vlocation = AbstractShellView::kExterior;
  else if (y < Theme::GetShadowMargin().top + kResizingMargin.top)
    vlocation = AbstractShellView::kResizeTop;
  else if (y < Theme::GetShadowMargin().top + GetHeight() - kResizingMargin.bottom)
    vlocation = AbstractShellView::kInterior;
  else if (y < Theme::GetShadowMargin().top + GetHeight() + kResizingMargin.bottom)
    vlocation = AbstractShellView::kResizeBottom;
  else
    vlocation = AbstractShellView::kExterior;

  location = vlocation | hlocation;
  if (location & AbstractShellView::kExterior)
    location = AbstractShellView::kExterior;

  if (location == AbstractShellView::kInterior &&
      y < Theme::GetShadowMargin().top + TitleBar::kHeight)
    location = AbstractShellView::kTitleBar;
  else if (location == AbstractShellView::kInterior)
    location = AbstractShellView::kClientArea;

  return location;
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
