#include <Raygen/Math/math.h>
#include <Raygen/Math/random.h>

#include <iostream>
#include <random>

int main() {
  float x, y, z;
  bool no_error = true;
  for (int i = 0; i < 1000; i++) {
    x = Raygen::random_real<float>(-2.0f, 2.0f);
    y = Raygen::random_real<float>(-2.0f, 2.0f);
    z = Raygen::random_real<float>(-2.0f, 2.0f);
    Eigen::Vector3f vec(x, y, z);

    float norm = vec.norm();
    float phi = Raygen::PolarAnglePhi(vec);
    float theta = Raygen::PloarAngleTheta(vec);

    Eigen::Vector3f ans = Raygen::Vector3FromPolar(phi, theta) * norm;
    float error = (ans - vec).norm();
    if (error > 1e-3) {
      std::cout << "error on vector:\n"
                << vec << "\n with the polar angle:" << phi * RAD2DEG << " "
                << theta * RAD2DEG << "\n with the result:\n"
                << ans << "\nwith error: " << error << "\n\n";
      no_error = false;
    }
  }
  if (no_error) std::cout << "pass\n";
}
