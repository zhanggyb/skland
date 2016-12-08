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

#include <skland/gui/main-window.hpp>

#include <skland/core/types.hpp>

#include <skland/gui/application.hpp>
#include <skland/gui/output.hpp>
#include <skland/gui/surface.hpp>
#include <skland/gui/memory-pool.hpp>
#include <skland/gui/buffer.hpp>
#include <skland/gui/mouse-event.hpp>
#include <skland/gui/key-event.hpp>

#include <skland/gui/window-frame.hpp>

#include <skland/gui/label.hpp>

namespace skland {

MainWindow::MainWindow(const char *title, int flags)
    : MainWindow(400, 300, title, flags) {
}

MainWindow::MainWindow(int width, int height, const char *title, int flags)
    : AbstractWindow(width, height, title, flags),
//      buffer_sub_(nullptr),
      window_frame_(nullptr),
      resize_location_(0),
      pool_(nullptr),
      buffer_(nullptr),
      redraw_needed_(true),
      main_widget_(nullptr) {
  set_name(title);  // debug only
  Initialize();
}

MainWindow::~MainWindow() {
  delete main_widget_;
  delete pool_;
  delete window_frame_;
  delete buffer_;
//  delete buffer_sub_;
}

void MainWindow::SetMainWidget(AbstractWidget *widget) {
  if (main_widget_) {
    delete main_widget_;
  }

  main_widget_ = widget;
  if (main_widget_) ResizeMainWidget(width(), height());
}

void MainWindow::OnMouseEnter(MouseEvent *event) {
//  DBG_PRINT_MSG("%s\n", "pointer enter");
  int location = window_frame_->GetPointerLocation(event);
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
    default: {
      event->SetCursor(Display::cursor(kCursorLeftPtr));
      break;
    }
  }
  event->Accept();
}

void MainWindow::OnMouseLeave(MouseEvent *event) {
//  DBG_PRINT_MSG("%s\n", "pointer leave");
//  int location = window_frame_->GetLocation((int) pointer->x(), (int) pointer->y());
//  print_location(location);
  event->Accept();
}

void MainWindow::OnMouseMove(MouseEvent *event) {

  int location = window_frame_->GetPointerLocation(event);
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
    default: {
      event->SetCursor(Display::cursor(kCursorLeftPtr));
      break;
    }
  }
  event->Accept();
}

void MainWindow::OnMouseButton(MouseEvent *event) {
  if ((event->button() == kMouseButtonLeft) &&
      (event->state() == kMouseButtonPressed)) {

    resize_location_ = window_frame_->GetPointerLocation(event);

    if (resize_location_ == kTitleBar) {
      xdg_toplevel_.Move(event->wl_seat(), event->serial());
      return;
    }

    if (resize_location_ < kResizeMask) {
      xdg_toplevel_.Resize(event->wl_seat(), event->serial(), (uint32_t) resize_location_);
    }
  }
  event->Accept();
}

void MainWindow::OnKeyboardKey(KeyEvent *event) {
  if (event->key() == kKey_ESC) {
    Application::Exit();
  }
  event->Accept();
}

void MainWindow::OnResize(int width, int height) {

  if (width < 200 || height < 200) return;

  resize(width, height);

  int total_width = this->width();
  int total_height = this->height();

  int input_region_x = 0;
  int input_region_y = 0;
  int input_region_width = this->width();
  int input_region_height = this->height();
  int opaque_region_x = 0;
  int opaque_region_y = 0;
  int opaque_region_width = this->width();
  int opaque_region_height = this->height();

  /*
  if (flags() & WindowFramelessMask) {
    window_frame_ = new WindowFrame(this, 0, AbstractWindowFrame::kNone);
  } else {
    window_frame_ = new WindowFrame(this);
    total_width += window_frame_->kShadowMargin.hsum();
    total_height += window_frame_->kShadowMargin.vsum();
    input_region_x += window_frame_->kShadowMargin.left() - window_frame_->kResizingMargin.left();
    input_region_y += window_frame_->kShadowMargin.top() - window_frame_->kResizingMargin.top();
    input_region_width += window_frame_->kResizingMargin.hsum();
    input_region_height += window_frame_->kResizingMargin.vsum();
    opaque_region_x += window_frame_->kShadowMargin.left();
    opaque_region_y += window_frame_->kShadowMargin.top();
  }
  */

  int total_size = total_width * 4 * total_height;
  if (total_size > pool_->size()) {
    DBG_PRINT_MSG("size_required: %d, pool size: %d, %s\n", total_size, pool_->size(), "Re-generate shm pool");
    pool_->Setup(total_size);
  }

  buffer_->Setup(*pool_,
                 total_width,
                 total_height,
                 total_width * 4,
                 WL_SHM_FORMAT_ARGB8888);
  surface()->Attach(*buffer_);

  input_region_.Setup(Display::wl_compositor());
  input_region_.Add(input_region_x, input_region_y, input_region_width, input_region_height);
  surface()->SetInputRegion(input_region_);

  opaque_region_.Setup(Display::wl_compositor());
  opaque_region_.Add(opaque_region_x, opaque_region_y, opaque_region_width, opaque_region_height);
  surface()->SetOpaqueRegion(opaque_region_);

// Test sub surface

//  Surface *sub = new Surface;
//  sub->set_name("Sub surface");
//  surface()->AddSubSurface(sub);
//  sub->SetPosition(100, 100);

//  buffer_sub_ = SHMPool::CreateBuffer(200, 200, 200 * 4, WL_SHM_FORMAT_ARGB8888);
//  sub->Attach(buffer_sub_);
//  sub->commit();

  surface()->Damage(0, 0, total_width, total_height);

  int x = AbstractWindowFrame::kShadowMargin.left;
  int y = AbstractWindowFrame::kShadowMargin.top;
  int w = this->width();
  int h = this->height();

  xdg_surface_.SetWindowGeometry(x, y, w, h);

  RedrawAll();

//    resize(width, height);
//    buffer_ = SHMPool::CreateBuffer(width,
//                                    height,
//                                    width * 4,
//                                    WL_SHM_FORMAT_ARGB8888);
//    surface()->Attach(buffer_);
//    surface()->Damage(0, 0, width, height);
//  DeepRedraw();
}

void MainWindow::OnDraw(Canvas *context) {

  /*
  uint32_t *pixel = (uint32_t *) buffer_->pixel();
  for (int n = 0; n < buffer_->geometry().width() * buffer_->geometry().height(); n++) {
    *pixel++ = 0x9F292929;
  }

  for (int y = window_frame_->margin().top();
       y < (buffer_->geometry().height() - window_frame_->margin().bottom());
       y++) {
    for (int x = window_frame_->margin().left();
         x < (buffer_->geometry().width() - window_frame_->margin().right());
         x++) {
      pixel = (uint32_t *) buffer_->address() + y * buffer_->geometry().width() + x;
      *pixel = 0xF0BBC0C0;
    }
  }

  for (int y = 20; y < (buffer_->geometry().height() - 20); y++) {
    for (int x = 20; x < (buffer_->geometry().width() - 20); x++) {
      pixel = (uint32_t *) buffer_->address() + y * buffer_->geometry().width() + x;
      *pixel = 0x9F1F1FBF;
    }
  }
  */

//  Timer::SaveCurrentTime();

//  uint32_t *pixel = (uint32_t *) buffer_->pixel();

//  for (int y = window_frame_->kShadowMargin.top();
//       y < (buffer_->geometry().height() - window_frame_->kShadowMargin.bottom());
//       y++) {
//    for (int x = window_frame_->kShadowMargin.left();
//         x < (buffer_->geometry().width() - window_frame_->kShadowMargin.right());
//         x++) {
//      pixel = (uint32_t *) buffer_->address() + y * buffer_->geometry().width() + x;
//      *pixel = 0xF0BBC0C0;
//    }
//  }

//  window_frame_->DropShadow(surface()->context());
//  FastBlur(pixel,
//           geometry().width() + window_frame_->kShadowMargin.hsum(),
//           geometry().height() + window_frame_->kShadowMargin.vsum(),
//           window_frame_->kShadowBlurRadius,
//           window_frame_->kShadowMargin);
  window_frame_->Draw(surface()->canvas());
//  double t = Timer::GetIntervalOfMilliseconds();
//  DBG_PRINT_MSG("blur time: %f\n", t);

//  xdg_surface_->set_window_geometry(geometry());
}

void MainWindow::Initialize() {
  Surface *surface = new Surface(this);
  SetSurface(surface);

  xdg_surface_.Setup(Display::xdg_shell(), surface->wl_surface());
  xdg_surface_.configure().Set(this, &MainWindow::OnXdgSurfaceConfigure);

  xdg_toplevel_.Setup(xdg_surface_);
  xdg_toplevel_.configure().Set(this, &MainWindow::OnXdgToplevelConfigure);
  xdg_toplevel_.close().Set(this, &MainWindow::OnXdgToplevelClose);

  Size output_size(1024, 800);
  if (const Output *output = Display::GetOutputAt(0)) {
    output_size = output->current_mode_size();
  }

  int total_width = std::max(width(), output_size.width);
  int total_height = std::max(height(), output_size.height);

  int input_region_x = 0;
  int input_region_y = 0;
  int input_region_width = width();
  int input_region_height = height();
  int opaque_region_x = 0;
  int opaque_region_y = 0;
  int opaque_region_width = width();
  int opaque_region_height = height();

  if (flags() & kWindowFramelessMask) {
    window_frame_ = new WindowFrame(this, 0, AbstractWindowFrame::kTitleBarNone);
  } else {
    window_frame_ = new WindowFrame(this);
    total_width += window_frame_->kShadowMargin.lr();
    total_height += window_frame_->kShadowMargin.tb();
    input_region_x += window_frame_->kShadowMargin.left - window_frame_->kResizingMargin.left;
    input_region_y += window_frame_->kShadowMargin.top - window_frame_->kResizingMargin.top;
    input_region_width += window_frame_->kResizingMargin.lr();
    input_region_height += window_frame_->kResizingMargin.tb();
    opaque_region_x += window_frame_->kShadowMargin.left;
    opaque_region_y += window_frame_->kShadowMargin.top;
  }

  input_region_.Setup(Display::wl_compositor());
  input_region_.Add(input_region_x, input_region_y, input_region_width, input_region_height);
  surface->SetInputRegion(input_region_);

  opaque_region_.Setup(Display::wl_compositor());
  opaque_region_.Add(opaque_region_x, opaque_region_y, opaque_region_width, opaque_region_height);
  surface->SetOpaqueRegion(opaque_region_);

  surface->Commit();

  pool_ = new MemoryPool;
  pool_->Setup(total_width * 4 * total_height);
  buffer_ = new Buffer;
  buffer_->Setup(*pool_,
                 total_width,
                 total_height,
                 total_width * 4,
                 WL_SHM_FORMAT_ARGB8888);

  Label *label1 = new Label("widget1", 200, 150);
  label1->SetPosition(100, 100);
  AddSubView(label1);

  Label *label2 = new Label("widget2", 180, 220);
  label2->SetPosition(400, 230);
  AddSubView(label2);
}

void MainWindow::OnFrameDone(uint32_t event) {
//  frame_callback_.reset(surface()->CreateFrameCallback());
//  frame_callback_->done().set(this, &MainWindow::OnFrameDone);

  DBG_PRINT_MSG("%s\n", "frame callback");
//  DeepRedraw();
}

void MainWindow::OnXdgSurfaceConfigure(uint32_t serial) {
  xdg_surface_.AckConfigure(serial);

  if (redraw_needed_) {
    int x = AbstractWindowFrame::kShadowMargin.left;
    int y = AbstractWindowFrame::kShadowMargin.top;
    int w = width();
    int h = height();

    xdg_surface_.SetWindowGeometry(x, y, w, h);

    surface()->Attach(*buffer_);
    RedrawAll();
    redraw_needed_ = false;
  }
}

void MainWindow::OnXdgToplevelConfigure(int width, int height, int states) {
  using namespace wayland::client;

  bool maximized = ((states & XdgToplevel::kStateMaskMaximized) != 0);
  bool fullscreen = ((states & XdgToplevel::kStateMaskFullscreen) != 0);
  bool resizing = ((states & XdgToplevel::kStateMaskResizing) != 0);
//  bool focus = ((states & XdgToplevel::kStateMaskActivated) != 0);

  if (maximized || fullscreen || resizing) {
    if (width != this->width() || height != this->height())
      OnResize(width, height);
  }
}

void MainWindow::OnXdgToplevelClose() {
  DBG_PRINT_MSG("%s\n", "toplevel close");
}

void MainWindow::ResizeMainWidget(int width, int height) {
  int x = kPadding;
  int y = kPadding;
  int w = width - 2 * kPadding;
  int h = height - 2 * kPadding;

  if (window_frame_) {
    // TODO: title bar position
    y += window_frame_->title_bar_size();
    h -= window_frame_->title_bar_size();
  }

  main_widget_->Resize(w, h);
  main_widget_->SetPosition(x, y);
}

}
