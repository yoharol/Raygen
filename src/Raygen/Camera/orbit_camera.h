#ifndef RAYGEN_CAMERA_ORBIT_CAMERA_H_
#define RAYGEN_CAMERA_ORBIT_CAMERA_H_

#include <raylib.h>
#include <Eigen/Core>
#include <memory>
#include "../Math/math.h"

namespace Raygen {

class OrbitCameraControl {
 private:
  Eigen::Vector3f dir;
  float rot_angle = 0;
  float tilt_angle = 0;
  float cam_dist = 5;  // how far away from the target the camera is (radius)
  float rot_speed;     // to scale the mouse input
  float mouse_wheel_speed;
  const float tilt_limit = 89.f * DEG2RAD;

 public:
  explicit OrbitCameraControl(const Camera3D& cam, float irot_speed = 6.f,
                              float imouse_wheel_speed = 1.f)
      : rot_speed(irot_speed * DEG2RAD), mouse_wheel_speed(imouse_wheel_speed) {
    dir = cast_vec3_r2e(cam.target) - cast_vec3_r2e(cam.position);
    cam_dist = dir.norm();
    rot_angle = polar_angle_phi(dir);
    tilt_angle = polar_angle_theta(dir);
  }

  void update(Camera3D& cam, Eigen::Vector2f cursor_delta,
              float mouse_wheel = 0) {
    rot_angle -= cursor_delta.x() * rot_speed;
    tilt_angle += cursor_delta.y() * rot_speed;
    if (tilt_angle > PI / 2 + tilt_limit) tilt_angle = PI / 2 + tilt_limit;
    if (tilt_angle < PI / 2 - tilt_limit) tilt_angle = PI / 2 - tilt_limit;
    cam_dist -= mouse_wheel;
    if (cam_dist < 1.f) cam_dist = 1.f;
    dir = vec3_from_polar(rot_angle, tilt_angle);
    cam.position = cast_vec3_e2r(cast_vec3_r2e(cam.target) - dir * cam_dist);
  }

  Eigen::Vector3f getForward() { return dir; }
};

}  // namespace Raygen

#endif  // RAYGEN_CAMERA_ORBIT_CAMERA_H_
