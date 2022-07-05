#ifndef RAYGEN_MATH_RAYGEN_MATH_
#define RAYGEN_MATH_RAYGEN_MATH_

#include <raylib.h>
#include <Eigen/Core>
#include <Eigen/Geometry>
#include <cmath>

namespace Raygen {

inline Eigen::Vector2f rescale_vec2(Eigen::Vector2f pos, float scale_x,
                                    float scale_y) {
  return Eigen::Vector2f(pos(0) * scale_x, pos(1) * scale_y);
}

/**
 * @brief Convert raylib Vector3 to Eigen::Vector3f
 *
 * @param vec raylib Vector3
 * @return Eigen::Vector3f
 */
inline Eigen::Vector3f cast_vec3_r2e(Vector3 vec) {
  return Eigen::Vector3f(vec.x, vec.y, vec.z);
}

/**
 * @brief Convert Eigen::Vector3f to raylib Vector3
 *
 * @param vec Eigen::Vector3f
 * @return raylib Vector3
 */
inline Vector3 cast_vec3_e2r(Eigen::Vector3f vec) {
  return {vec.x(), vec.y(), vec.z()};
}

/**
 * @brief Convert raylib Vector2 to Eigen::Vector2f
 *
 * @param vec raylib Vector2
 * @return Eigen::Vector2f
 */
inline Eigen::Vector2f cast_vec2_r2e(Vector2 vec) {
  return Eigen::Vector2f(vec.x, vec.y);
}

/**
 * @brief Convert Eigen::Vector2f to raylib Vector2
 *
 * @param vec Eigen::Vector2f
 * @return raylib Vector2
 */
inline Vector2 cast_vec2_e2r(Eigen::Vector2f vec) { return {vec.x(), vec.y()}; }

/**
 * @brief return unsigned angle between 2 Eigen::Vector3f
 */
inline float angle_between(Eigen::Vector3f vec1, Eigen::Vector3f vec2) {
  vec1.normalize();
  vec2.normalize();
  return acos(vec1.dot(vec2));
}

/**
 * @brief return phi angle of Eigen::Vector3f
 *
 * @return phi start from +x axis
 */
inline float polar_angle_phi(Eigen::Vector3f vec) {
  vec.y() = 0;
  float sign = ((vec.z() < 0) ? 1.f : -1.f);
  return sign * angle_between(Eigen::Vector3f::UnitX(), vec);
}

/**
 * @brief return theta angle of Eigen::Vector3f
 *
 * @return theta start from +y axis
 */
inline float polar_angle_theta(Eigen::Vector3f vec) {
  return angle_between(Eigen::Vector3f::UnitY(), vec);
}

/**
 * @brief Retrive Eigen::Vector3f from polar angles
 */
inline Eigen::Vector3f vec3_from_polar(float phi, float theta) {
  Eigen::Vector3f form = Eigen::Vector3f::UnitY();
  return Eigen::AngleAxisf(phi, Eigen::Vector3f::UnitY()) *
         Eigen::AngleAxisf(theta, -Eigen::Vector3f::UnitZ()) *
         Eigen::Vector3f::UnitY();
}

}  // namespace Raygen

#endif  // RAYGEN_MATH_RAYGEN_MATH_
