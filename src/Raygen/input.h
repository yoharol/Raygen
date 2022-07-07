#ifndef RAYGEN_INPUT_H_
#define RAYGEN_INPUT_H_

#include <raylib.h>
#include "Math/math.h"

namespace Raygen {
Eigen::Vector2f get_mouse_position() {
  return cast_vec2_r2e(GetMousePosition());
}

Eigen::Vector2f get_mouse_delta() { return cast_vec2_r2e(GetMouseDelta()); }
}  // namespace Raygen

#endif  // RAYGEN_INPUT_H_
