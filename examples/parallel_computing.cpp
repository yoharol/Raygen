// #define RAYGEN_PARALLEL_LOG_INFO_
#include <Raygen/parallel.h>
#include <Raygen/Type/type.h>
#include <Raygen/Math/random.h>
#include <Raygen/timer.h>
#include <Eigen/Core>
#include <vector>

using Raygen::parallel_for;
using Raygen::parallel_for_2d;

int main() {
  printf("[testing parallel_for]");
  const int n = 50;
  const int k_debug = 10;
  std::vector<Eigen::Vector2f> eigen_list(50);
  parallel_for(n, [&eigen_list](const int i) {
    float fi = static_cast<float>(i);
    eigen_list[i] = Eigen::Vector2f(fi, fi * fi);
    // eigen_list[i] = eigen_list[i] * eigen_list[i];
  });
  for (int i = 0; i < k_debug; i++) {
    const int x = static_cast<int>(Raygen::random_real<float>(0, n));
    printf("%d : (%g, %g)\n", x, eigen_list[x](0), eigen_list[x](1));
  }

  printf("[testing parallel for 2d]\n");
  const int width = 10;
  const int height = 10;
  Raygen::d2array<Eigen::Vector2f> array_2d(width, height);
  parallel_for_2d(width, height, [&array_2d](const int i, const int j) {
    array_2d(i, j) = Eigen::Vector2f(i, j);
  });
  for (int i = 0; i < k_debug; i++) {
    const int x = static_cast<int>(Raygen::random_real<float>(0, width));
    const int y = static_cast<int>(Raygen::random_real<float>(0, height));
    printf("%d, %d: (%g, %g)\n", x, y, array_2d(x, y)(0), array_2d(x, y)(1));
  }

  Raygen::Timer timer;
  timer.setTimer();
  const int size_long = 10000;
  std::vector<float> array(size_long);
  for (int i = 0; i < size_long; i++) {
    for (int j = 0; j < size_long; j++)
      array[i] += sqrt(static_cast<double>(j));
    array[i] /= size_long;
  }
  timer.endTimer("Unparalleled Computing");

  timer.setTimer();
  Raygen::parallel_for(size_long, [&](const int i) {
    for (int j = 0; j < size_long; j++)
      array[i] += sqrt(static_cast<double>(j));
    array[i] /= size_long;
  });
  timer.endTimer("Paralleled Computing");
}
