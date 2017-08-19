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
#include "skland/core/property.hpp"

#include "skland/gui/key-event.hpp"
#include "skland/gui/mouse-event.hpp"

#include "skland/gui/surface.hpp"
#include "skland/gui/callback.hpp"
#include "skland/gui/title-bar.hpp"
#include "skland/gui/glesv2-api.hpp"

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

struct GLWindow::Private : public core::Property<GLWindow> {

  SKLAND_DECLARE_NONCOPYABLE_AND_NONMOVALE(Private);

  explicit Private(GLWindow *window)
      : core::Property<GLWindow>(window) {}

  ~Private() final = default;

  /* Properties for frame surface, JUST experimental */
  SharedMemoryPool pool;

  Buffer frame_buffer;

  AbstractGRAPI *gr_api = nullptr;

  Surface *gl_surface = nullptr;

  Callback frame_callback;

  int GetMouseLocation(const MouseEvent *event) const;

  void DrawFrame(const Context &context);

  void OnFrame(uint32_t serial);

//  bool animating = false;

};

void GLWindow::Private::DrawFrame(const Context &context) {
  Canvas *canvas = context.canvas();
  canvas->Clear();

  Path path;
  RectF geometry = RectF::MakeFromXYWH(0.f, 0.f, owner->GetWidth(), owner->GetHeight());

  if ((!owner->IsMaximized()) || (!owner->IsFullscreen())) {
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
    owner->DropShadow(context);
    canvas->Restore();
  } else {
    path.AddRect(geometry);
  }

//   Fill color:
  /*
  Paint paint;
  paint.SetAntiAlias(true);
  paint.SetColor(0xEFF0F0F0);
  canvas->DrawPath(path, paint);
   */

  // Draw the client area:
//  paint.SetColor(0xEFE0E0E0);
//  canvas->Save();
//  canvas->ClipPath(path, kClipIntersect, true);
//  canvas->DrawRect(Rect::FromXYWH(0.f, 0.f, GetWidth(), GetHeight()), paint);
//  canvas->Restore();

  canvas->Flush();
}

int GLWindow::Private::GetMouseLocation(const MouseEvent *event) const {
  int vlocation, hlocation, location;
  int x = static_cast<int>(event->GetSurfaceXY().x),
      y = static_cast<int>(event->GetSurfaceXY().y);

  // TODO: maximized or frameless

  if (x < (Theme::GetShadowMargin().left - kResizingMargin.left))
    hlocation = AbstractShellView::kExterior;
  else if (x < Theme::GetShadowMargin().left + kResizingMargin.left)
    hlocation = AbstractShellView::kResizeLeft;
  else if (x < Theme::GetShadowMargin().left + owner->GetWidth() - kResizingMargin.right)
    hlocation = AbstractShellView::kInterior;
  else if (x < Theme::GetShadowMargin().left + owner->GetWidth() + kResizingMargin.right)
    hlocation = AbstractShellView::kResizeRight;
  else
    hlocation = AbstractShellView::kExterior;

  if (y < (Theme::GetShadowMargin().top - kResizingMargin.top))
    vlocation = AbstractShellView::kExterior;
  else if (y < Theme::GetShadowMargin().top + kResizingMargin.top)
    vlocation = AbstractShellView::kResizeTop;
  else if (y < Theme::GetShadowMargin().top + owner->GetHeight() - kResizingMargin.bottom)
    vlocation = AbstractShellView::kInterior;
  else if (y < Theme::GetShadowMargin().top + owner->GetHeight() + kResizingMargin.bottom)
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

void GLWindow::Private::OnFrame(uint32_t serial) {
  frame_callback.Setup(*gl_surface);
  owner->OnRender();
  gl_surface->Commit();
}

GLWindow::GLWindow(const char *title)
    : GLWindow(400, 300, title) {
}

GLWindow::GLWindow(int width, int height, const char *title)
    : AbstractShellView(width, height, title, nullptr) {
  p_ = core::make_unique<Private>(this);

  p_->frame_callback.done().Set(p_.get(), &Private::OnFrame);
}

GLWindow::~GLWindow() {
  delete p_->gr_api;
  delete p_->gl_surface;
}

void GLWindow::OnShown() {
  Surface *shell_surface = this->GetShellSurface();
  const Margin &margin = shell_surface->GetMargin();

  // Create buffer and attach it to the shell surface:
  int width = GetWidth() + margin.lr();  // buffer width with horizontal margins
  int height = GetHeight() + margin.tb();  // buffer height with vertical margins
  int32_t pool_size = width * 4 * height;

  p_->pool.Setup(pool_size);
  p_->frame_buffer.Setup(p_->pool, width, height,
                         width * 4, WL_SHM_FORMAT_ARGB8888);
  shell_surface->Attach(&p_->frame_buffer);
  shell_surface->Update();

  // Create a sub surface and use it as a gl surface for 3D
  p_->gl_surface = Surface::Sub::Create(shell_surface, this);

  Region region;  // zero region
  p_->gl_surface->SetInputRegion(region);

  p_->gr_api = new GLESV2API;  // Currently use GLES for demo
  p_->gl_surface->SetGRAPI(p_->gr_api);
  p_->gr_api->SetViewportSize(GetWidth(), GetHeight());

  Surface::Sub::Get(p_->gl_surface)->SetWindowPosition(0, 0);

  p_->gl_surface->Update();
}

void GLWindow::OnRequestUpdate(AbstractView *view) {

}

void GLWindow::OnConfigureSize(const Size &old_size, const Size &new_size) {
  if (new_size.width < 160) {
    RequestSaveSize(false);
    return;
  }

  if (new_size.height < 120) {
    RequestSaveSize(false);
    return;
  }

  if (old_size == new_size) {
    RequestSaveSize(false);
    return;
  }

  Surface::Shell::Get(GetShellSurface())->ResizeWindow(new_size.width, new_size.height);  // Call xdg surface api

  p_->gr_api->SetViewportSize(new_size.width, new_size.height);
  OnResize(new_size.width, new_size.height);

  DispatchMouseLeaveEvent();
  RequestSaveSize(true);
}

void GLWindow::OnSaveSize(const Size &old_size, const Size &new_size) {
  Surface *shell_surface = this->GetShellSurface();
  const Margin &margin = shell_surface->GetMargin();
  int width = new_size.width;
  int height = new_size.height;

  RectI input_rect(width, height);

  input_rect.left = margin.left - kResizingMargin.left;
  input_rect.top = margin.top - kResizingMargin.top;
  input_rect.Resize(width + kResizingMargin.lr(), height + kResizingMargin.tb());

  Region input_region;
  input_region.Add(input_rect.x(), input_rect.y(),
                   input_rect.width(), input_rect.height());
  shell_surface->SetInputRegion(input_region);

  // Reset buffer:
  width += margin.lr();
  height += margin.tb();

  int pool_size = width * 4 * height;
  p_->pool.Setup(pool_size);

  p_->frame_buffer.Setup(p_->pool, width, height, width * 4, WL_SHM_FORMAT_ARGB8888);
  shell_surface->Attach(&p_->frame_buffer);

  shell_surface->Update();
}

void GLWindow::OnRenderSurface(Surface *surface) {
  if (surface == p_->gl_surface) {
    p_->frame_callback.Setup(*p_->gl_surface);
    OnInitialize();
    p_->gl_surface->Commit();
    return;
  }

  Surface *shell_surface = GetShellSurface();
  const Margin &margin = shell_surface->GetMargin();

  Canvas canvas((unsigned char *) p_->frame_buffer.GetData(),
                p_->frame_buffer.GetSize().width,
                p_->frame_buffer.GetSize().height);
  canvas.SetOrigin(margin.left, margin.top);
  p_->DrawFrame(Context(shell_surface, &canvas));
  shell_surface->Damage(0, 0, GetWidth() + margin.lr(), GetHeight() + margin.tb());
  shell_surface->Commit();
}

void GLWindow::OnMouseMove(MouseEvent *event) {
//  AbstractView *view = nullptr;
  switch (p_->GetMouseLocation(event)) {
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

//  if (nullptr != view) DispatchMouseEnterEvent(view, event);
}

void GLWindow::OnMouseDown(MouseEvent *event) {
  if ((event->GetButton() == MouseButton::kMouseButtonLeft) &&
      (event->GetState() == MouseButtonState::kMouseButtonPressed)) {

    int location = p_->GetMouseLocation(event);

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

void GLWindow::OnFocus(bool focus) {
  GetShellSurface()->Update();
}

void GLWindow::OnInitialize() {

}

void GLWindow::OnResize(int /*width*/, int /*height*/) {

}

void GLWindow::OnRender() {
  p_->gr_api->MakeCurrent();

  glClearColor(0.f, 0.f, 0.f, 1.f);
  glClear(GL_COLOR_BUFFER_BIT);
  glFlush();

  p_->gr_api->SwapBuffers();
}

bool GLWindow::MakeCurrent() {
  p_->gr_api->MakeCurrent();
  return false;
}

void GLWindow::SwapBuffers() {
  p_->gr_api->SwapBuffers();
}

} // namespace gui
} // namespace skland
