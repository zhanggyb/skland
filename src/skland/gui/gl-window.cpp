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

#include "skland/gui/gl-window.hpp"

#include "skland/core/assert.hpp"
#include "skland/core/memory.hpp"

#include "skland/gui/key-event.hpp"
#include "skland/gui/mouse-event.hpp"

#include "skland/gui/surface.hpp"
#include "skland/gui/callback.hpp"
#include "skland/gui/title-bar.hpp"
#include "skland/gui/glesv2-interface.hpp"

#include "skland/gui/shared-memory-pool.hpp"
#include "skland/gui/buffer.hpp"
#include "skland/gui/region.hpp"

#include "internal/display_proxy.hpp"
#include "internal/abstract-view_iterators.hpp"

#include "skland/graphic/canvas.hpp"
#include "skland/graphic/paint.hpp"
#include "skland/graphic/path.hpp"

#include "skland/gui/theme.hpp"

#include "SkCanvas.h"

#include <GLES2/gl2.h>

namespace skland {
namespace gui {

using core::RectF;
using core::RectI;
using core::SizeI;
using graphic::Canvas;
using graphic::Paint;
using graphic::Path;
using graphic::ClipOperation;

struct GLWindow::Private {

  SKLAND_DECLARE_NONCOPYABLE_AND_NONMOVALE(Private);

  Private() = default;

  ~Private() = default;

  /* Properties for frame surface, JUST experimental */
  SharedMemoryPool pool;

  Buffer frame_buffer_;
//  std::unique_ptr<Canvas> frame_canvas;

//  Surface::EGL *egl_surface = nullptr;

//  GLESV2Interface *gl_interface = nullptr;
//  Surface *gl_surface = nullptr;

//  Callback frame_callback;

//  bool animating = false;

};

GLWindow::GLWindow(const char *title)
    : GLWindow(400, 300, title) {
}

GLWindow::GLWindow(int width, int height, const char *title)
    : AbstractShellView(width, height, title, nullptr) {
  p_ = core::make_unique<Private>();
}

GLWindow::~GLWindow() {
//  delete p_->gl_interface;
//  delete p_->gl_surface;
}

void GLWindow::OnShown() {
  Surface *shell_surface = this->GetShellSurface();

  // Create buffer:
  int width = GetWidth();
  int height = GetHeight();
  width += shell_surface->GetMargin().lr();
  height += shell_surface->GetMargin().tb();

  int32_t pool_size = width * 4 * height;

  p_->pool.Setup(pool_size);

  p_->frame_buffer_.Setup(p_->pool, width, height,
                          width * 4, WL_SHM_FORMAT_ARGB8888);
  shell_surface->Attach(&p_->frame_buffer_);
  Canvas canvas((unsigned char *) p_->frame_buffer_.GetData(),
                p_->frame_buffer_.GetSize().width,
                p_->frame_buffer_.GetSize().height);
//  p_->frame_canvas->SetOrigin((float) shell_surface->GetMargin().left,
//                              (float) shell_surface->GetMargin().top);
  canvas.Clear();
  canvas.Flush();

  shell_surface->Update();
}

void GLWindow::OnRequestUpdate(AbstractView *view) {

}

void GLWindow::OnConfigureSize(const Size &old_size, const Size &new_size) {
  SizeI min(160, 120);
  SizeI max(65536, 65536);
  _ASSERT(min.width < max.width && min.height < max.height);

  if (new_size.width < min.width || new_size.height < min.height) {
    RequestSaveSize(false);
    return;
  }
  if (new_size.width > max.width || new_size.height > max.height) {
    RequestSaveSize(false);
    return;
  }

  if (old_size == new_size) {
    RequestSaveSize(false);
    return;
  }

  Surface::Shell::Get(GetShellSurface())->ResizeWindow(GetWidth(), GetHeight());  // Call xdg surface api

  DispatchMouseLeaveEvent();
  RequestSaveSize();
}

void GLWindow::OnSaveSize(const Size &old_size, const Size &new_size) {
  int width = new_size.width;
  int height = new_size.height;

  RectI input_rect(width, height);
  Surface *shell_surface = this->GetShellSurface();

  input_rect.left = shell_surface->GetMargin().left - kResizingMargin.left;
  input_rect.top = shell_surface->GetMargin().top - kResizingMargin.top;
  input_rect.Resize(width + kResizingMargin.lr(),
                    height + kResizingMargin.tb());

  Region input_region;
  input_region.Add(input_rect.x(), input_rect.y(),
                   input_rect.width(), input_rect.height());
  shell_surface->SetInputRegion(input_region);

  // Reset buffer:
  width += shell_surface->GetMargin().lr();
  height += shell_surface->GetMargin().tb();

  int pool_size = width * 4 * height;
  p_->pool.Setup(pool_size);

  p_->frame_buffer_.Setup(p_->pool, width, height, width * 4, WL_SHM_FORMAT_ARGB8888);
  shell_surface->Attach(&p_->frame_buffer_);
  Canvas canvas((unsigned char *) p_->frame_buffer_.GetData(),
                p_->frame_buffer_.GetSize().width,
                p_->frame_buffer_.GetSize().height);
//  p_->frame_canvas->SetOrigin(shell_surface->GetMargin().left, shell_surface->GetMargin().top);
  canvas.Clear();
  canvas.Flush();

//  const core::Margin &margin = shell_surface->GetMargin();
//  RedrawTask *redraw_task = RedrawTask::Get(this);
//  redraw_task->context = Context(shell_surface, p_->frame_canvas.get());
//  PushBackIdleTask(redraw_task);
//  Damage(this,
//         0, 0,
//         GetWidth() + margin.lr(),
//         GetHeight() + margin.tb());
  shell_surface->Update();
}

void GLWindow::OnRenderSurface(Surface *surface) {
  Surface *shell_surface = GetShellSurface();
  Canvas canvas((unsigned char *) p_->frame_buffer_.GetData(),
                p_->frame_buffer_.GetSize().width,
                p_->frame_buffer_.GetSize().height);
  canvas.SetOrigin(shell_surface->GetMargin().left, shell_surface->GetMargin().top);
  OnDraw(Context(shell_surface, &canvas));
  shell_surface->Damage(0,
                        0,
                        GetWidth() + shell_surface->GetMargin().lr(),
                        GetHeight() + shell_surface->GetMargin().tb());
  shell_surface->Commit();
}

void GLWindow::OnMouseMove(MouseEvent *event) {
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
//      view = p_->title_bar;
      break;
    }
    case kClientArea: {
      event->SetCursor(Display::GetCursor(kCursorLeftPtr));
//      view = p_->content_view;
      break;
    }
    default: {
      event->SetCursor(Display::GetCursor(kCursorLeftPtr));
      break;
    }
  }

  if (view) DispatchMouseEnterEvent(view, event);
}

void GLWindow::OnMouseDown(MouseEvent *event) {
  if ((event->GetButton() == MouseButton::kMouseButtonLeft) &&
      (event->GetState() == MouseButtonState::kMouseButtonPressed)) {

    int location = GetMouseLocation(event);

    if (location == kClientArea && (nullptr == EventTask::GetMouseTask(this)->GetNext())) {
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

void GLWindow::OnMouseUp(MouseEvent *event) {

}

void GLWindow::OnKeyDown(KeyEvent *event) {

}

void GLWindow::OnDraw(const Context &context) {
  Canvas *canvas = context.canvas();
  canvas->Clear();

  Path path;
  RectF geometry = RectF::MakeFromXYWH(0.f, 0.f, GetWidth(), GetHeight());

  if ((!IsMaximized()) || (!IsFullscreen())) {
    // Drop shadow:
//    float radii[] = {
//        7.f, 7.f, // top-left
//        7.f, 7.f, // top-right
//        4.f, 4.f, // bottom-right
//        4.f, 4.f  // bottom-left
//    };
//    path.AddRoundRect(geometry, radii);
    path.AddRect(geometry);
    canvas->Save();
    canvas->ClipPath(path, ClipOperation::kClipDifference, true);
    DropShadow(&context);
    canvas->Restore();
  } else {
    path.AddRect(geometry);
  }

//   Fill color:
  Paint paint;
  paint.SetAntiAlias(true);
  paint.SetColor(0xEFF0F0F0);
  canvas->DrawPath(path, paint);

  // Draw the client area:
//  paint.SetColor(0xEFE0E0E0);
//  canvas->Save();
//  canvas->ClipPath(path, kClipIntersect, true);
//  canvas->DrawRect(Rect::FromXYWH(0.f, 0.f, GetWidth(), GetHeight()), paint);
//  canvas->Restore();

  canvas->Flush();
}

void GLWindow::OnFocus(bool focus) {
  GetShellSurface()->Update();
}

void GLWindow::OnInitialize() {

}

void GLWindow::OnResize(int /*width*/, int /*height*/) {

}

void GLWindow::OnRender() {
//  p_->egl_surface->MakeCurrent();

  glClearColor(0.f, 0.f, 0.f, 1.f);
  glClear(GL_COLOR_BUFFER_BIT);
  glFlush();

//  p_->egl_surface->SwapBuffers();
}

bool GLWindow::MakeCurrent() {
//  p_->gl_surface->GetGLInterface()->MakeCurrent();
  return false;
}

void GLWindow::SwapBuffers() {
//  p_->gl_surface->GetGLInterface()->SwapBuffers();
}

int GLWindow::GetMouseLocation(const MouseEvent *event) const {
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

//  if (location == AbstractShellView::kInterior &&
//      y < Theme::GetShadowMargin().top + 22 /* title_bar_size_ */)
//    location = AbstractShellView::kTitleBar;
//  else
  if (location == AbstractShellView::kInterior)
    location = AbstractShellView::kClientArea;

  return location;
}

void GLWindow::OnFrame(uint32_t serial) {
//  p_->frame_callback.Setup(*p_->gl_surface);
//  OnRender();
//  p_->gl_surface->Commit();
}

} // namespace gui
} // namespace skland
