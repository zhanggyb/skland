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
#include <skland/gui/mouse-event.hpp>
#include <skland/gui/key-event.hpp>
#include <skland/gui/title-bar.hpp>

#include <skland/gui/shared-memory-pool.hpp>
#include <skland/gui/buffer.hpp>
#include <skland/gui/region.hpp>
#include <skland/gui/output.hpp>

#include <skland/gui/theme.hpp>

#include <skland/graphic/canvas.hpp>
#include <skland/graphic/paint.hpp>
#include <skland/graphic/path.hpp>
#include <skland/graphic/gradient-shader.hpp>

namespace skland {
namespace gui {

using core::PointF;
using core::RectF;
using core::RectI;
using core::Margin;
using core::Deque;

using graphic::Canvas;
using graphic::Paint;
using graphic::Path;
using graphic::Shader;
using graphic::GradientShader;
using graphic::ClipOperation;

/**
 * @ingroup gui_intern
 * @brief The private structure for Window
 */
SKLAND_NO_EXPORT struct Window::Private {

  SKLAND_DECLARE_NONCOPYABLE(Private);

  Private()
      : main_surface(nullptr),
        title_bar(nullptr),
        content_view(nullptr),
        minimal_size(160, 120),
        preferred_size(640, 480),
        maximal_size(65536, 65536),
        flags(0),
        output(nullptr),
        inhibit_update(true) {}

  ~Private() {}

  /** The surface for rendering views in this window */
  Surface *main_surface;

  SharedMemoryPool pool;

  Buffer frame_buffer;
  std::unique_ptr<Canvas> frame_canvas;

  Buffer main_buffer;
  std::unique_ptr<Canvas> main_canvas;

  /** The default title bar */
  TitleBar *title_bar;

  AbstractView *content_view;

  Size minimal_size;

  Size preferred_size;

  Size maximal_size;

  int flags;

  const Output *output;

  bool inhibit_update;

};

// --------------

Window::Window(const char *title, int flags)
    : Window(640, 480, title, flags) {
}

Window::Window(int width, int height, const char *title, int flags)
    : AbstractShellView(width, height, title, nullptr) {
  p_.reset(new Private);
  p_->flags = flags;

  Surface *shell_surface = GetShellSurface();

  if (!(flags & kFlagMaskFrameless)) {
    // Create a sub surface for views in this window:
    p_->main_surface = Surface::Sub::Create(shell_surface, this, Theme::GetShadowMargin());
    _ASSERT(p_->main_surface->GetParent() == shell_surface);
    _ASSERT(p_->main_surface->GetSiblingBelow() == shell_surface);
    Region empty_region;
    p_->main_surface->SetInputRegion(empty_region);

    // Create the default title bar:
    TitleBar *titlebar = new TitleBar;
    p_->title_bar = titlebar;
    AttachView(p_->title_bar);

    titlebar->SetTitle(title);
    titlebar->Resize(GetWidth(), TitleBar::kHeight);

    AbstractButton *button = titlebar->GetButton(TitleBar::kButtonClose);
    button->clicked().Connect(this, static_cast<void (Window::*)(core::SLOT)>(&AbstractShellView::Close));

    button = titlebar->GetButton(TitleBar::kButtonMaximize);
    button->clicked().Connect(this, static_cast<void (Window::*)(core::SLOT)>(&AbstractShellView::ToggleMaximize));

    button = titlebar->GetButton(TitleBar::kButtonMinimize);
    button->clicked().Connect(this, static_cast<void (Window::*)(core::SLOT)>(&AbstractShellView::Minimize));

    button = titlebar->GetButton(TitleBar::kButtonFullscreen);
    //button->clicked().Connect(this, static_cast<void (Window::*)(SLOT)>(&AbstractShellView::SetFullscreen));
    button->clicked().Connect(this, &Window::OnFullscreenButtonClicked);
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
  SetContentViewGeometry();
}

const core::SizeI &Window::GetMinimalSize() const {
  return p_->minimal_size;
}

const core::SizeI &Window::GetPreferredSize() const {
  return p_->preferred_size;
}

const core::SizeI &Window::GetMaximalSize() const {
  return p_->maximal_size;
}

void Window::OnShown() {
  Surface *shell_surface = GetShellSurface();

  int scale = 1;
  if (nullptr == p_->output) {
    const Deque &outputs = Display::GetOutputs();
    if (outputs.count()) {
      p_->output = static_cast<const Output *>(outputs[0]);
    }
  }
  if (p_->output) scale = p_->output->GetScale();
  shell_surface->SetScale(scale);

  // Create buffer:
  int width = GetWidth() * scale;
  int height = GetHeight() * scale;
  const Margin &margin = shell_surface->GetMargin();
  width += margin.lr() * scale;
  height += margin.tb() * scale;

  int32_t pool_size = width * 4 * height;
  if (p_->main_surface) pool_size *= 2; // double buffer for 2 surfaces, not frameless

  p_->pool.Setup(pool_size);

  p_->frame_buffer.Setup(p_->pool, width, height,
                         width * 4, WL_SHM_FORMAT_ARGB8888);
  shell_surface->Attach(&p_->frame_buffer);
  p_->frame_canvas.reset(Canvas::CreateRasterDirect(width, height,
                                                    (unsigned char *) p_->frame_buffer.GetData()));
  p_->frame_canvas->SetOrigin((float) margin.left * scale,
                              (float) margin.top * scale);
  p_->frame_canvas->Clear();

  if (p_->main_surface) {
    p_->main_surface->SetScale(scale);
    p_->main_buffer.Setup(p_->pool, width, height,
                          width * 4, WL_SHM_FORMAT_ARGB8888,
                          width * 4 * height);
    p_->main_surface->Attach(&p_->main_buffer);
    p_->main_canvas.reset(Canvas::CreateRasterDirect(width, height,
                                                     (unsigned char *) p_->main_buffer.GetData()));
    p_->main_canvas->SetOrigin((float) margin.left * scale,
                               (float) margin.top * scale);
    p_->main_canvas->Clear();
  }

  p_->inhibit_update = false;

  RequestUpdate();
  if (p_->title_bar) {
    AbstractShellView::DispatchUpdate(p_->title_bar);
    p_->title_bar->Resize(GetWidth(), TitleBar::kHeight);
  }
  if (p_->content_view) {
    AbstractShellView::DispatchUpdate(p_->content_view);
    SetContentViewGeometry();
  }
}

void Window::OnUpdate(AbstractView *view) {
  if (p_->inhibit_update) return;

  Surface *surface = nullptr;
  Canvas *canvas = nullptr;

  if (p_->main_surface) {
    surface = p_->main_surface;
    canvas = p_->main_canvas.get();
  } else {
    surface = this->GetShellSurface();
    canvas = p_->frame_canvas.get();
  }

  AbstractView::RedrawTask *redraw_task = AbstractView::RedrawTask::Get(view);
  redraw_task->context = Context(surface, canvas);
  PushToTail(redraw_task);
  _ASSERT(canvas);
  Damage(view,
         view->GetX() + surface->GetMargin().left,
         view->GetY() + surface->GetMargin().top,
         view->GetWidth(),
         view->GetHeight());
  surface->Commit();
}

Surface *Window::GetSurface(const AbstractView *view) const {
  if (nullptr == view)
    return GetShellSurface();

  return nullptr != p_->main_surface ? p_->main_surface : GetShellSurface();
}

bool Window::OnConfigureSize(const Size &old_size, const Size &new_size) {
  _ASSERT(p_->minimal_size.width < p_->maximal_size.width &&
      p_->minimal_size.height < p_->maximal_size.height);

  if ((new_size.width < p_->minimal_size.width) ||
      (new_size.height < p_->minimal_size.height))
    return false;

  if ((new_size.width > p_->maximal_size.width) ||
      (new_size.height > p_->maximal_size.height))
    return false;

  RedrawTask *redraw_task = RedrawTask::Get(this);

  if (old_size == new_size) {
    redraw_task->Unlink();
    return false;
  }

  p_->inhibit_update = true;
  PushToTail(redraw_task);

  Surface::Shell::Get(GetShellSurface())->ResizeWindow(GetWidth(), GetHeight());  // Call xdg surface api
  // surface size is changed, reset the pointer position and enter/leave widgets
  DispatchMouseLeaveEvent();

  return true;
}

void Window::OnSizeChange(const Size &old_size, const Size &new_size) {
  Surface *shell_surface = this->GetShellSurface();

  int scale = 1;
  const Deque &outputs = Display::GetOutputs();
  if (outputs.count()) {
    p_->output = static_cast<const Output *>(outputs[0]);
    scale = p_->output->GetScale();
  }
  shell_surface->SetScale(scale);

  int width = new_size.width;
  int height = new_size.height;
  const core::Margin &margin = shell_surface->GetMargin();

  RectI input_rect(width, height);

  input_rect.left = margin.left - kResizingMargin.left;
  input_rect.top = margin.top - kResizingMargin.top;
  input_rect.Resize(width + kResizingMargin.lr(),
                    height + kResizingMargin.tb());

  Region input_region;
  input_region.Add(input_rect.x(), input_rect.y(),
                   input_rect.width(), input_rect.height());
  shell_surface->SetInputRegion(input_region);

  // Reset buffer:
  width *= scale;
  height *= scale;
  width += margin.lr() * scale;
  height += margin.tb() * scale;

  int pool_size = width * 4 * height;
  if (p_->main_surface) pool_size *= 2;

  p_->pool.Setup(pool_size);

  p_->frame_buffer.Setup(p_->pool, width, height, width * 4, WL_SHM_FORMAT_ARGB8888);
  shell_surface->Attach(&p_->frame_buffer);
  p_->frame_canvas.reset(Canvas::CreateRasterDirect(width, height,
                                                    (unsigned char *) p_->frame_buffer.GetData()));
  p_->frame_canvas->SetOrigin(margin.left * scale, margin.top * scale);
  p_->frame_canvas->Clear();

  if (p_->main_surface) {
    p_->main_buffer.Setup(p_->pool, width, height, width * 4, WL_SHM_FORMAT_ARGB8888, width * 4 * height);
    p_->main_surface->Attach(&p_->main_buffer);
    p_->main_canvas.reset(Canvas::CreateRasterDirect(width, height,
                                                     (unsigned char *) p_->main_buffer.GetData()));
    p_->main_canvas->SetOrigin(margin.left * scale,
                               margin.top * scale);
    p_->main_canvas->Clear();
  }

  // Set context before draw this window
  RedrawTask::Get(this)->context = Context(shell_surface, p_->frame_canvas.get());

  Damage(this,
         0, 0,
         GetWidth() + margin.lr(),
         GetHeight() + margin.tb());

  shell_surface->Commit();

  p_->inhibit_update = false;
  if (p_->title_bar) {
    DispatchUpdate(p_->title_bar);
    p_->title_bar->Resize(new_size.width, TitleBar::kHeight);
  }
  if (p_->content_view) {
    DispatchUpdate(p_->content_view);
    SetContentViewGeometry();
  }
}

void Window::OnMouseEnter(MouseEvent *event) {
  AbstractView *view = nullptr;
  int location = GetMouseLocation(event);
  switch (location) {
    case kResizeTop: {
      event->SetCursor(Display::GetCursor(kCursorTop));
      break;
    }
    case kResizeBottom: {
      event->SetCursor(Display::GetCursor(kCursorBottom));
      break;
    }
    case kResizeLeft: {
      event->SetCursor(Display::GetCursor(kCursorLeft));
      break;
    }
    case kResizeRight: {
      event->SetCursor(Display::GetCursor(kCursorRight));
      break;
    }
    case kResizeTopLeft: {
      event->SetCursor(Display::GetCursor(kCursorTopLeft));
      break;
    }
    case kResizeTopRight: {
      event->SetCursor(Display::GetCursor(kCursorTopRight));
      break;
    }
    case kResizeBottomLeft: {
      event->SetCursor(Display::GetCursor(kCursorBottomLeft));
      break;
    }
    case kResizeBottomRight: {
      event->SetCursor(Display::GetCursor(kCursorBottomRight));
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
      event->SetCursor(Display::GetCursor(kCursorLeftPtr));
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
      event->SetCursor(Display::GetCursor(kCursorTop));
      break;
    }
    case kResizeBottom: {
      DispatchMouseLeaveEvent();
      event->SetCursor(Display::GetCursor(kCursorBottom));
      break;
    }
    case kResizeLeft: {
      DispatchMouseLeaveEvent();
      event->SetCursor(Display::GetCursor(kCursorLeft));
      break;
    }
    case kResizeRight: {
      DispatchMouseLeaveEvent();
      event->SetCursor(Display::GetCursor(kCursorRight));
      break;
    }
    case kResizeTopLeft: {
      DispatchMouseLeaveEvent();
      event->SetCursor(Display::GetCursor(kCursorTopLeft));
      break;
    }
    case kResizeTopRight: {
      DispatchMouseLeaveEvent();
      event->SetCursor(Display::GetCursor(kCursorTopRight));
      break;
    }
    case kResizeBottomLeft: {
      DispatchMouseLeaveEvent();
      event->SetCursor(Display::GetCursor(kCursorBottomLeft));
      break;
    }
    case kResizeBottomRight: {
      DispatchMouseLeaveEvent();
      event->SetCursor(Display::GetCursor(kCursorBottomRight));
      break;
    }
    case kTitleBar: {
      event->SetCursor(Display::GetCursor(kCursorLeftPtr));
      view = p_->title_bar;
      break;
    }
    case kClientArea: {
      event->SetCursor(Display::GetCursor(kCursorLeftPtr));
      view = p_->content_view;
      break;
    }
    default: {
      event->SetCursor(Display::GetCursor(kCursorLeftPtr));
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

    if (location == kTitleBar && (nullptr == EventTask::GetMouseTask(this)->next())) {
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

  int scale = context->surface()->GetScale();
  int width = GetWidth() * scale;
  int height = GetHeight() * scale;

  Path path;
  RectF geometry = RectF::MakeFromXYWH(0.f, 0.f, width, height);
  bool drop_shadow = !(IsMaximized() || IsFullscreen());
  const Theme::Schema &window_schema = Theme::GetData().window;
  const Theme::Schema &title_bar_schema = Theme::GetData().title_bar;
  Shader shader;
  PointF points[2];
  points[0].x = 0.f;
  points[0].y = 0.f;
  points[1].x = 0.f;
  points[1].y = height;

  if (drop_shadow) {
    geometry = geometry.Inset(-0.5f * scale);
    float radii[] = {
        7.f * scale, 7.f * scale, // top-left
        7.f * scale, 7.f * scale, // top-right
        4.f * scale, 4.f * scale, // bottom-right
        4.f * scale, 4.f * scale  // bottom-left
    };
    path.AddRoundRect(geometry, radii);
    Canvas::ClipGuard guard(canvas, path, ClipOperation::kClipDifference, true);
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
  Canvas::ClipGuard guard(canvas, path, ClipOperation::kClipIntersect, true);

  paint.SetStyle(Paint::Style::kStyleFill);
  if (p_->title_bar) {
    if (title_bar_schema.active.background.shaded) {
      points[1].y = TitleBar::kHeight * scale;
      shader = GradientShader::MakeLinear(points,
                                          title_bar_schema.active.background.shaded_colors.data(),
                                          title_bar_schema.active.background.shaded_positions.data(),
                                          title_bar_schema.active.background.shaded_count,
                                          Shader::TileMode::kTileModeClamp);
      paint.SetShader(shader);
    } else {
      paint.SetColor(title_bar_schema.active.background.color);
    }
    canvas->DrawRect(p_->title_bar->GetGeometry() * scale, paint);
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
  canvas->DrawRect(GetContentGeometry() * scale, paint);
  canvas->Flush();
}

void Window::OnFocus(bool focus) {
  RequestUpdate();
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

void Window::OnEnterOutput(const Surface *surface, const Output *output) {
  if (p_->output == output) return;

  p_->output = output;
  Surface *shell_surface = GetShellSurface();
  if (surface == shell_surface) {
    shell_surface->SetScale(output->GetScale());
  } else if (surface == p_->main_surface) {
    p_->main_surface->SetScale(output->GetScale());
  }
}

void Window::OnLeaveOutput(const Surface *surface, const Output *output) {
  if (p_->output != output) return;

  p_->output = nullptr;

  int scale = 1;
  const Deque &outputs = Display::GetOutputs();
  if (outputs.count()) {
    p_->output = static_cast<const Output *>(outputs[0]);
    scale = p_->output->GetScale();
  }

  GetShellSurface()->SetScale(scale);
  if (p_->main_surface) p_->main_surface->SetScale(scale);
}

int Window::GetMouseLocation(const MouseEvent *event) const {
  int vlocation, hlocation, location;
  int x = static_cast<int>(event->GetSurfaceXY().x),
      y = static_cast<int>(event->GetSurfaceXY().y);

  // TODO: maximized or fullscreen

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

RectI Window::GetContentGeometry() const {
  int x = 0;
  int y = 0;
  int w = GetWidth();
  int h = GetHeight();
  if (p_->title_bar) {
    y += p_->title_bar->GetHeight();
    h -= p_->title_bar->GetHeight();
  }
  return RectI::MakeFromXYWH(x, y, w, h);
}

void Window::OnFullscreenButtonClicked(core::SLOT slot) {
  if (IsFullscreen()) {
    ToggleFullscreen(nullptr);
  } else {
    if (p_->output)
      ToggleFullscreen(p_->output);
    else {
      const Deque &outputs = Display::GetOutputs();
      if (outputs.count()) {
        p_->output = static_cast<const Output *>(outputs[0]);
        ToggleFullscreen(p_->output);
      }
    }
  }
}

void Window::SetContentViewGeometry() {
  const RectI geometry = GetContentGeometry();
  p_->content_view->MoveTo(geometry.x(), geometry.y());
  p_->content_view->Resize(geometry.width(), geometry.height());
}

void Window::RequestUpdate() {
  RedrawTask *redraw_task = RedrawTask::Get(this);

  Surface *shell_surface = GetShellSurface();
  const core::Margin &margin = shell_surface->GetMargin();

  redraw_task->context = Context(shell_surface, p_->frame_canvas.get());
  PushToTail(redraw_task);
  _ASSERT(p_->frame_canvas);
  Damage(this,
         0, 0,
         GetWidth() + margin.lr(),
         GetHeight() + margin.tb());
  shell_surface->Commit();
}

void Window::CancelUpdate() {
  RedrawTask *redraw_task = RedrawTask::Get(this);
  redraw_task->Unlink();
}

} // namespace gui
} // namespace skland
