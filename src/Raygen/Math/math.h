#ifndef RAYGEN_MATH_RAYGEN_MATH_
#define RAYGEN_MATH_RAYGEN_MATH_

#include <raylib.h>
#include <Eigen/Core>
#include <Eigen/Geometry>
#include <cmath>

namespace Raygen {

/**
 * @brief Convert raylib Vector3 to Eigen::Vector3f
 *
 * @param vec raylib Vector3
 * @return Eigen::Vector3f
 */
inline Eigen::Vector3f CastVec3_R2E(Vector3 vec) {
  return Eigen::Vector3f(vec.x, vec.y, vec.z);
}

/**
 * @brief Convert Eigen::Vector3f to raylib Vector3
 *
 * @param vec Eigen::Vector3f
 * @return raylib Vector3
 */
inline Vector3 CastVec3_E2R(Eigen::Vector3f vec) {
  return {vec.x(), vec.y(), vec.z()};
}

/**
 * @brief Convert raylib Vector2 to Eigen::Vector2f
 *
 * @param vec raylib Vector2
 * @return Eigen::Vector2f
 */
inline Eigen::Vector2f CastVec2_R2E(Vector2 vec) {
  return Eigen::Vector2f(vec.x, vec.y);
}

/**
 * @brief Convert Eigen::Vector2f to raylib Vector2
 *
 * @param vec Eigen::Vector2f
 * @return raylib Vector2
 */
inline Vector2 CastVec2_E2R(Eigen::Vector2f vec) { return {vec.x(), vec.y()}; }

/**
 * @brief return unsigned angle between 2 Eigen::Vector3f
 */
inline float AngleBetween(Eigen::Vector3f vec1, Eigen::Vector3f vec2) {
  vec1.normalize();
  vec2.normalize();
  return acos(vec1.dot(vec2));
}

/**
 * @brief return phi angle of Eigen::Vector3f
 *
 * @return phi start from +x axis
 */
inline float PolarAnglePhi(Eigen::Vector3f vec) {
  vec.y() = 0;
  float sign = ((vec.z() < 0) ? 1.f : -1.f);
  return sign * AngleBetween(Eigen::Vector3f::UnitX(), vec);
}

/**
 * @brief return theta angle of Eigen::Vector3f
 *
 * @return theta start from +y axis
 */
inline float PloarAngleTheta(Eigen::Vector3f vec) {
  return AngleBetween(Eigen::Vector3f::UnitY(), vec);
}

/**
 * @brief Retrive Eigen::Vector3f from polar angles
 */
inline Eigen::Vector3f Vector3FromPolar(float phi, float theta) {
  Eigen::Vector3f form = Eigen::Vector3f::UnitY();
  return Eigen::AngleAxisf(phi, Eigen::Vector3f::UnitY()) *
         Eigen::AngleAxisf(theta, -Eigen::Vector3f::UnitZ()) *
         Eigen::Vector3f::UnitY();
}

}  // namespace Raygen

#endif  // RAYGEN_MATH_RAYGEN_MATH_
