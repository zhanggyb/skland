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

#ifndef SKLAND_WAYLAND_XDG_POSITIONER_HPP_
#define SKLAND_WAYLAND_XDG_POSITIONER_HPP_

#include "xdg-shell.hpp"

namespace skland {
namespace wayland {

class XdgPopup;
struct XdgPositionerMeta;

class XdgPositioner {

  friend class XdgPopup;

  XdgPositioner(const XdgPositioner &) = delete;
  XdgPositioner &operator=(const XdgPositioner &) = delete;

 public:

  enum AnchorType {
    /**
     * the center of the anchor rectangle
     */
        kAnchorNone = 0, //  ZXDG_POSITIONER_V6_ANCHOR_NONE = 0

    /**
     * the top edge of the anchor rectangle
     */
        kAnchorTop = 1, //    ZXDG_POSITIONER_V6_ANCHOR_TOP = 1,

    /**
     * the bottom edge of the anchor rectangle
     */
        kAnchorBottom = 2,  //    ZXDG_POSITIONER_V6_ANCHOR_BOTTOM = 2,

    /**
     * the left edge of the anchor rectangle
     */
        kAnchorLeft = 4, //   ZXDG_POSITIONER_V6_ANCHOR_LEFT = 4,

    /**
     * the right edge of the anchor rectangle
     */
        kAnchorRight = 8 //   ZXDG_POSITIONER_V6_ANCHOR_RIGHT = 8,
  };

  enum GravityType {
    /**
     * center over the anchor edge
     */
        kGravityNone = 0, //    ZXDG_POSITIONER_V6_GRAVITY_NONE = 0,

    /**
     * position above the anchor edge
     */
        kGravityTop = 1,  //    ZXDG_POSITIONER_V6_GRAVITY_TOP = 1,

    /**
     * position below the anchor edge
     */
        kGravityBottom = 2, //    ZXDG_POSITIONER_V6_GRAVITY_BOTTOM = 2,

    /**
     * position to the left of the anchor edge
     */
        kGravityLeft = 4, //    ZXDG_POSITIONER_V6_GRAVITY_LEFT = 4,

    /**
     * position to the right of the anchor edge
     */
        kGravityRight = 8,  //    ZXDG_POSITIONER_V6_GRAVITY_RIGHT = 8,
  };

  enum ConstraintAdjustmentType {
    kConstraintAdjustmentNone = 0,  // ZXDG_POSITIONER_V6_CONSTRAINT_ADJUSTMENT_NONE = 0,
    kConstraintAdjustmentSlideX = 1, // ZXDG_POSITIONER_V6_CONSTRAINT_ADJUSTMENT_SLIDE_X = 1,
    kConstraintAdjustmentSlideY = 2,  // ZXDG_POSITIONER_V6_CONSTRAINT_ADJUSTMENT_SLIDE_Y = 2,
    kConstraintAdjustmentFlipX = 4, // ZXDG_POSITIONER_V6_CONSTRAINT_ADJUSTMENT_FLIP_X = 4,
    kConstraintAdjustmentFlipY = 8, // ZXDG_POSITIONER_V6_CONSTRAINT_ADJUSTMENT_FLIP_Y = 8,
    kConstraintAdjustmentResizeX = 16,  //ZXDG_POSITIONER_V6_CONSTRAINT_ADJUSTMENT_RESIZE_X = 16,
    kConstraintAdjustmentResizeY = 32 // ZXDG_POSITIONER_V6_CONSTRAINT_ADJUSTMENT_RESIZE_Y = 32,
  };

  XdgPositioner();

  ~XdgPositioner();

  void Setup(const XdgShell &xdg_shell);

  void Destroy();

  void SetSize(int width, int height);

  void SetAnchorRect(int32_t x, int32_t y, int32_t width, int32_t height);

  void SetAnchor(uint32_t anchor);

  void SetGravity(uint32_t gravity);

  void SetConstraintAdjustment(uint32_t constraint_adjustment);

  void SetOffset(int32_t x, int32_t y);

  void SetUserData(void *user_data);

  void *GetUserData() const;

  uint32_t GetVersion() const;

  bool IsValid() const;

 private:

  std::unique_ptr<XdgPositionerMeta> metadata_;

};

} // namespace wayland
} // namespace skland

#endif // SKLAND_WAYLAND_XDG_POSITIONER_HPP_
