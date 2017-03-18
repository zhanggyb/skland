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

#include <skland/graphic/canvas.hpp>

#include "internal/display-registry.hpp"
#include "internal/abstract-shell-view-redraw-task-iterator.hpp"
#include "internal/abstract-view-redraw-task-iterator.hpp"

namespace skland {

Window::Window(const char *title, AbstractShellFrame *frame)
    : Window(640, 480, title, frame) {
}

Window::Window(int width, int height, const char *title, AbstractShellFrame *frame)
    : AbstractShellView(width, height, title, nullptr, frame),
      main_surface_(nullptr) {
  if (frame) {
    Surface *parent = GetShellSurface();
    main_surface_ = SubSurface::Create(parent, this, Theme::shadow_margin());
    DBG_ASSERT(main_surface_->parent() == parent);
    DBG_ASSERT(main_surface_->below() == parent);
    wayland::Region empty_region;
    empty_region.Setup(Display::Registry().wl_compositor());
    main_surface_->SetInputRegion(empty_region);
  }
}

Window::~Window() {
  delete main_surface_;
}

void Window::SetContentView(AbstractView *view) {
  SetClientView(view);
}

void Window::OnShown() {
  Surface *shell_surface = this->GetShellSurface();

  // Create buffer:
  int width = GetSize().width;
  int height = GetSize().height;
  width += shell_surface->margin().lr();
  height += shell_surface->margin().tb();

  int32_t pool_size = width * 4 * height;
  if (main_surface_) pool_size *= 2; // double buffer for 2 surfaces

  pool_.Setup(pool_size);

  frame_buffer_.Setup(pool_, width, height,
                      width * 4, WL_SHM_FORMAT_ARGB8888);
  shell_surface->Attach(&frame_buffer_);
  frame_canvas_.reset(new Canvas((unsigned char *) frame_buffer_.GetData(),
                                 frame_buffer_.GetSize().width,
                                 frame_buffer_.GetSize().height));
  frame_canvas_->SetOrigin((float) shell_surface->margin().left,
                           (float) shell_surface->margin().top);
  frame_canvas_->Clear();

  if (main_surface_) {
    main_buffer_.Setup(pool_, width, height,
                       width * 4, WL_SHM_FORMAT_ARGB8888,
                       width * 4 * height);
    main_surface_->Attach(&main_buffer_);
    main_canvas_.reset(new Canvas((unsigned char *) main_buffer_.GetData(),
                                  main_buffer_.GetSize().width,
                                  main_buffer_.GetSize().height));
    main_canvas_->SetOrigin((float) main_surface_->margin().left,
                            (float) main_surface_->margin().top);
    main_canvas_->Clear();
  }

  OnUpdate(nullptr);
  AbstractView *title_view = GetTitleView();
  if (title_view) RecursiveUpdate(title_view);
  if (GetClientView()) RecursiveUpdate(GetClientView());
}

void Window::OnUpdate(AbstractView *view) {
  if (!IsShown()) return;

  Surface *surface = nullptr;

  if (nullptr == view) {
    surface = this->GetShellSurface();
    RedrawTaskIterator it(this);
    it.PushToTail();
    it.SetContext(Context(surface, frame_canvas_));
    DBG_ASSERT(frame_canvas_);
    Damage(this,
           0, 0,
           GetSize().width + surface->margin().lr(),
           GetSize().height + surface->margin().tb());
    surface->Commit();
  } else {
    std::shared_ptr<Canvas> canvas;
    if (main_surface_) {
      surface = main_surface_;
      canvas = main_canvas_;
    } else {
      surface = this->GetShellSurface();
      canvas = frame_canvas_;
    }

    AbstractView::RedrawTaskIterator it(view);
    it.PushToTail();
    it.SetContext(Context(surface, canvas));
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

  return nullptr != main_surface_ ? main_surface_ : GetShellSurface();
}

void Window::OnKeyboardKey(KeyEvent *event) {
  if (event->key() == kKey_ESC) {
    Application::Exit();
  }
  event->Accept();
}

void Window::OnResize(int /*old_width*/, int /*old_height*/, int new_width, int new_height) {
  int width = new_width;
  int height = new_height;

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
  if (main_surface_) pool_size *= 2;

  pool_.Setup(pool_size);

  frame_buffer_.Setup(pool_, width, height, width * 4, WL_SHM_FORMAT_ARGB8888);
  shell_surface->Attach(&frame_buffer_);
  frame_canvas_.reset(new Canvas((unsigned char *) frame_buffer_.GetData(),
                                 frame_buffer_.GetSize().width,
                                 frame_buffer_.GetSize().height));
  frame_canvas_->SetOrigin(shell_surface->margin().left, shell_surface->margin().top);
  frame_canvas_->Clear();

  if (main_surface_) {
    main_buffer_.Setup(pool_, width, height, width * 4, WL_SHM_FORMAT_ARGB8888, width * 4 * height);
    main_surface_->Attach(&main_buffer_);
    main_canvas_.reset(new Canvas((unsigned char *) main_buffer_.GetData(),
                                  main_buffer_.GetSize().width,
                                  main_buffer_.GetSize().height));
    main_canvas_->SetOrigin(main_surface_->margin().left,
                            main_surface_->margin().top);
    main_canvas_->Clear();
  }

  OnUpdate(nullptr);  // Update the background

  AbstractView *view = GetTitleView();
  if (view) RecursiveUpdate(view);

  view = GetClientView();
  if (view) RecursiveUpdate(view);
}

}
