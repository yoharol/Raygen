#ifndef RAYGEN_RANDOM_H_
#define RAYGEN_RANDOM_H_

#include <random>

namespace Raygen {

/**
 * @brief Return a random real number in [0, 1]
 */
template <typename T>
T random_real() {
  static std::uniform_real_distribution<T> distribution(0.0, 1.0);
  static std::mt19937 generator(std::random_device{}());
  return distribution(generator);
}

/**
 * @brief Return a random real number in [lb, ub]
 */
template <typename T>
T random_real(T lb, T ub) {
  return lb + (ub - lb) * random_real<T>();
}

}  // namespace Raygen
#endif  //  RAYGEN_RANDOM_H_
