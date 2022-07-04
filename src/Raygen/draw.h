#ifndef RAYGEN_DRAW_H_
#define RAYGEN_DRAW_H_

#include <raylib.h>
#include <Eigen/Core>
#include "Type/color.h"
#include "Math/math.h"

namespace Raygen {

inline void clear_background(const Color4f& color) {
  ClearBackground(color.toColor());
}

inline void draw_texture(const Texture2D& tex, Eigen::Vector2f pos,
                         const Color4f& color) {
  DrawTexture(tex, pos.x(), pos.y(), color.toColor());
}

inline void draw_line(Eigen::Vector2f startPos, Eigen::Vector2f endPos,
                      const Color4f& color, float thick = 1.0f) {
  DrawLineEx(cast_vec2_e2r(startPos), cast_vec2_e2r(endPos), thick,
             color.toColor());
}
}  // namespace Raygen

#endif  //  RAYGEN_DRAW_H_
