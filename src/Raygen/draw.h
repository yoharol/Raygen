#ifndef RAYGEN_DRAW_H_
#define RAYGEN_DRAW_H_

#include "Math/math.h"
#include "Type/color.h"
#include <Eigen/Core>
#include <raylib.h>

namespace Raygen {

inline void clear_background(const Color4f& color) { ClearBackground(color.toColor()); }

inline float get_screen_width() { return static_cast<float>(GetScreenWidth()); }
inline float get_screen_height() { return static_cast<float>(GetScreenHeight()); }

inline Eigen::Vector2f get_mouse_canvas_position()
{
    Eigen::Vector2f mouse_pos = cast_vec2_r2e(GetMousePosition());
    mouse_pos(0) /= get_screen_width();
    mouse_pos(1) = 1.0f - mouse_pos(1) / get_screen_height();
    return mouse_pos;
}

inline void
draw_texture(const Texture2D& tex, Eigen::Vector2f pos, const Color4f& color, float scalex = 1.0f, float scaley = 1.0f)
{
    DrawTexture(tex, pos.x() * scalex, get_screen_height() - pos.y() * scaley, color.toColor());
}

inline void draw_line(Eigen::Vector2f startPos,
                      Eigen::Vector2f endPos,
                      const Color4f&  color,
                      float           thick  = 1.0f,
                      float           scalex = 1.0f,
                      float           scaley = 1.0f)
{
    startPos(0) *= scalex;
    startPos(1) = get_screen_height() - startPos(1) * scaley;
    endPos(0) *= scalex;
    endPos(1) = get_screen_height() - endPos(1) * scaley;
    DrawLineEx(cast_vec2_e2r(startPos), cast_vec2_e2r(endPos), thick, color.toColor());
}
} // namespace Raygen

#endif //  RAYGEN_DRAW_H_
