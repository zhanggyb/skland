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

#include <skland/gui/egl-window.hpp>

#include <skland/gui/key-event.hpp>
#include <skland/gui/mouse-event.hpp>

#include <skland/gui/surface.hpp>
#include <skland/gui/callback.hpp>
#include <skland/gui/title-bar.hpp>

#include <skland/gui/shared-memory-pool.hpp>
#include <skland/gui/buffer.hpp>
#include <skland/gui/region.hpp>

#include "internal/display_native.hpp"
#include "internal/abstract-view_iterators.hpp"

#include <skland/graphic/canvas.hpp>
#include <skland/graphic/paint.hpp>
#include <skland/graphic/path.hpp>

#include <skland/gui/theme.hpp>

#include "SkCanvas.h"

#include <GLES2/gl2.h>
#include <skland/core/assert.hpp>

namespace skland {

using graphic::Canvas;
using graphic::Paint;
using graphic::Path;
using graphic::ClipOperation;

namespace gui {

struct EGLWindow::Private {

  Private(const Private &) = delete;
  Private &operator=(const Private &) = delete;

  Private()
      : sub_surface(nullptr),
        egl_surface(nullptr),
        animating(false) {}

  ~Private() {}

  /* Properties for frame surface, JUST experimental */
  SharedMemoryPool pool;

  Buffer frame_buffer_;
  std::unique_ptr<Canvas> frame_canvas;

  Surface *sub_surface;

  Surface::EGL *egl_surface;

  Callback frame_callback;

  bool animating;

};

EGLWindow::EGLWindow(const char *title)
    : EGLWindow(400, 300, title) {
}

EGLWindow::EGLWindow(int width, int height, const char *title)
    : AbstractShellView(width, height, title, nullptr) {
  p_.reset(new Private);

  Surface *parent = GetShellSurface();

  p_->sub_surface = Surface::Sub::Create(parent, this);
  _ASSERT(p_->sub_surface->GetParent() == parent);
  _ASSERT(p_->sub_surface->GetSiblingBelow() == parent);

  Region empty_region;
  p_->sub_surface->SetInputRegion(empty_region);

  Surface::Sub::Get(p_->sub_surface)->SetWindowPosition(0, 0);

  p_->egl_surface = Surface::EGL::Get(p_->sub_surface);
  p_->egl_surface->Resize(GetWidth(), GetHeight());

  p_->frame_callback.done().Set(this, &EGLWindow::OnFrame);
}

EGLWindow::~EGLWindow() {
  delete p_->egl_surface;
  delete p_->sub_surface;
}

void EGLWindow::OnShown() {
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
  p_->frame_canvas.reset(new Canvas((unsigned char *) p_->frame_buffer_.GetData(),
                                    p_->frame_buffer_.GetSize().width,
                                    p_->frame_buffer_.GetSize().height));
  p_->frame_canvas->SetOrigin((float) shell_surface->GetMargin().left,
                              (float) shell_surface->GetMargin().top);
  p_->frame_canvas->Clear();

  RequestUpdate();
}

void EGLWindow::OnUpdate(AbstractView *view) {
  return;
}

Surface *EGLWindow::GetSurface(const AbstractView *view) const {
  if (nullptr == view)
    return GetShellSurface();

  return nullptr != p_->sub_surface ? p_->sub_surface : GetShellSurface();
}

bool EGLWindow::OnConfigureSize(const Size &old_size, const Size &new_size) {
  Size min(160, 120);
  Size max(65536, 65536);
  _ASSERT(min.width < max.width && min.height < max.height);

  if (new_size.width < min.width || new_size.height < min.height) return false;
  if (new_size.width > max.width || new_size.height > max.height) return false;

  RedrawTask *redraw_task = RedrawTask::Get(this);

  if (old_size == new_size) {
    redraw_task->Unlink();
    return false;
  }

  PushToTail(redraw_task);

  Surface::Shell::Get(GetShellSurface())->ResizeWindow(GetWidth(), GetHeight());  // Call xdg surface api
  // surface size is changed, reset the pointer position and enter/leave widgets

  p_->egl_surface->Resize(new_size.width, new_size.height);
  OnResize(new_size.width, new_size.height);

  DispatchMouseLeaveEvent();

  return true;
}

void EGLWindow::OnSizeChange(const Size &old_size, const Size &new_size) {
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
  p_->frame_canvas.reset(new Canvas((unsigned char *) p_->frame_buffer_.GetData(),
                                    p_->frame_buffer_.GetSize().width,
                                    p_->frame_buffer_.GetSize().height));
  p_->frame_canvas->SetOrigin(shell_surface->GetMargin().left, shell_surface->GetMargin().top);
  p_->frame_canvas->Clear();

  const Margin &margin = shell_surface->GetMargin();
  RedrawTask *redraw_task = RedrawTask::Get(this);
  redraw_task->context = Context(shell_surface, p_->frame_canvas.get());
  PushToTail(redraw_task);
  _ASSERT(p_->frame_canvas);
  Damage(this,
         0, 0,
         GetWidth() + margin.lr(),
         GetHeight() + margin.tb());
  shell_surface->Commit();
}

void EGLWindow::OnMouseMove(MouseEvent *event) {
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

void EGLWindow::OnMouseDown(MouseEvent *event) {
  if ((event->GetButton() == MouseButton::kMouseButtonLeft) &&
      (event->GetState() == MouseButtonState::kMouseButtonPressed)) {

    int location = GetMouseLocation(event);

    if (location == kClientArea && (nullptr == EventTask::GetMouseTask(this)->next())) {
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

void EGLWindow::OnMouseUp(MouseEvent *event) {

}

void EGLWindow::OnKeyDown(KeyEvent *event) {

}

void EGLWindow::OnDraw(const Context *context) {
  Canvas *canvas = context->canvas();
  canvas->Clear();

  Path path;
  Rect geometry = Rect::FromXYWH(0.f, 0.f, GetWidth(), GetHeight());

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
    DropShadow(context);
    canvas->Restore();
  } else {
    path.AddRect(geometry);
  }

  // Fill color:
//  Paint paint;
//  paint.SetAntiAlias(true);
//  paint.SetColor(0xEFF0F0F0);
//  canvas->DrawPath(path, paint);

  // Draw the client area:
//  paint.SetColor(0xEFE0E0E0);
//  canvas->Save();
//  canvas->ClipPath(path, kClipIntersect, true);
//  canvas->DrawRect(Rect::FromXYWH(0.f, 0.f, GetWidth(), GetHeight()), paint);
//  canvas->Restore();

  canvas->Flush();

  if (!p_->animating) {
    p_->animating = true;
    p_->frame_callback.Setup(*p_->egl_surface->GetSurface());
    OnInitialize();
  }
}

void EGLWindow::OnFocus(bool) {
  RequestUpdate();
}

void EGLWindow::OnInitialize() {

}

void EGLWindow::OnResize(int /*width*/, int /*height*/) {

}

void EGLWindow::OnRender() {
  p_->egl_surface->MakeCurrent();

  glClearColor(0.f, 0.f, 0.f, 1.f);
  glClear(GL_COLOR_BUFFER_BIT);
  glFlush();

  p_->egl_surface->SwapBuffers();
}

bool EGLWindow::MakeCurrent() {
  return p_->egl_surface->MakeCurrent();
}

void EGLWindow::SwapBuffers() {
  p_->egl_surface->SwapBuffers();
}

int EGLWindow::GetMouseLocation(const MouseEvent *event) const {
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

void EGLWindow::OnFrame(uint32_t serial) {
  p_->frame_callback.Setup(*p_->egl_surface->GetSurface());
  OnRender();
  p_->egl_surface->GetSurface()->Commit();
}

void EGLWindow::OnRelease() {
//  fprintf(stderr, "on release\n");
}

void EGLWindow::RequestUpdate() {
  Surface *shell_surface = GetShellSurface();
  const Margin &margin = shell_surface->GetMargin();

  RedrawTask *redraw_task = RedrawTask::Get(this);
  redraw_task->context = Context(shell_surface, p_->frame_canvas.get());
  PushToTail(redraw_task);
  _ASSERT(p_->frame_canvas);
  Damage(this,
         0, 0,
         GetWidth() + margin.lr(),
         GetHeight() + margin.tb());
  shell_surface->Commit();
}

void EGLWindow::CancelUpdate() {
  RedrawTask *redraw_task = RedrawTask::Get(this);
  redraw_task->Unlink();
}

} // namespace gui
} // namespace skland
