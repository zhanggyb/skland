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

#include "skland/gui/dialog.hpp"

#include "skland/core/rect.hpp"
#include "skland/gui/shared-memory-pool.hpp"
#include "skland/gui/title-bar.hpp"
#include "skland/gui/buffer.hpp"
#include <skland/gui/region.hpp>

#include <skland/graphic/paint.hpp>
#include "skland/graphic/canvas.hpp"

namespace skland {
namespace gui {

using core::RectI;

using graphic::Canvas;

struct Dialog::Private {

  SKLAND_DECLARE_NONCOPYABLE_AND_NONMOVALE(Private);

  Private()
      : flags(0),
        title_bar(nullptr),
        content_view(nullptr) {}

  ~Private() = default;

  int flags;

  SharedMemoryPool pool;

  Buffer buffer;
  std::unique_ptr<Canvas> canvas;

  TitleBar *title_bar;

  AbstractView *content_view;

};

Dialog::Dialog(const char *title, AbstractShellView *parent)
    : AbstractShellView(title, parent) {
  p_.reset(new Private);
}

Dialog::~Dialog() {
  if (nullptr != p_->content_view) p_->content_view->Destroy();
  if (nullptr != p_->title_bar) p_->title_bar->Destroy();
}

void Dialog::OnShown() {
  Surface *shell_surface = GetShellSurface();

  shell_surface->SetScale(1);

  // Create buffer:
  int width = GetWidth();
  int height = GetHeight();

  int32_t pool_size = width * 4 * height;

  p_->pool.Setup(pool_size);
  p_->buffer.Setup(p_->pool, width, height, width * 4, WL_SHM_FORMAT_ARGB8888);
  shell_surface->Attach(&p_->buffer);
  p_->canvas.reset(Canvas::CreateRasterDirect(width, height,
                                              (unsigned char *) p_->buffer.GetData()));
  p_->canvas->Clear();

  shell_surface->Update(true);
  Damage(this, 0, 0, width, height);
  shell_surface->Commit();
}

void Dialog::OnConfigureSize(const Size &old_size, const Size &new_size) {
  SaveSize();
}

void Dialog::OnSaveSize(const Size &old_size, const Size &new_size) {
  Surface *shell_surface = GetShellSurface();

  shell_surface->SetScale(1);

  int width = new_size.width;
  int height = new_size.height;

  Region input_region;
  input_region.Add(0, 0, width, height);
  shell_surface->SetInputRegion(input_region);

  int pool_size = width * 4 * height;
  p_->pool.Setup(pool_size);

  p_->buffer.Setup(p_->pool, width, height, width * 4, WL_SHM_FORMAT_ARGB8888);
  shell_surface->Attach(&p_->buffer);
  p_->canvas.reset(Canvas::CreateRasterDirect(width, height,
                                              (unsigned char *) p_->buffer.GetData()));
  p_->canvas->Clear();

  shell_surface->Update(true);
  Damage(this, 0, 0, width, height);
  shell_surface->Commit();
}

void Dialog::RenderSurface(const Surface *surface) {
  using graphic::Paint;

  Surface *shell_surface = GetShellSurface();

  if (surface == shell_surface) {
    Paint paint;
    paint.SetColor(0xFFDFDF2F);
    p_->canvas->DrawRect(core::RectF(0.f, 0.f, GetWidth(), GetHeight()), paint);
    p_->canvas->Flush();

    shell_surface->Damage(0, 0, GetWidth(), GetHeight());
    shell_surface->Commit();
  }
}

} // namespace gui
} // namespace skland
