#ifndef RAYGEN_TYPE_D2ARRAY_H_
#define RAYGEN_TYPE_D2ARRAY_H_

#include <algorithm>
#include <cassert>
#include <vector>

namespace Raygen {

/**
 * @brief 2d array library based on std vector
 */
template <typename T>
class d2array {
 private:
  std::vector<T> _array;
  int _nx, _ny;

 public:
  /**
   * @brief Construct a new d2array object
   *
   * @param nx width
   * @param ny height
   */
  d2array(int nx, int ny) : _nx(nx), _ny(ny) { _array.resize(nx * ny); }

  /**
   * @brief Construct a new d2array object
   *
   * @param nx width
   * @param ny height
   * @param _value initial value
   */
  d2array(int nx, int ny, T value) : _nx(nx), _ny(ny) {
    _array.resize(nx * ny, value);
  }

  void resize(int nx, int ny) {
    _nx = nx;
    _ny = ny;
    _array.clear();
    _array.resize(nx * ny);
  }

  void resize(int nx, int ny, T value) {
    _nx = nx;
    _ny = ny;
    _array.clear();
    _array.resize(nx * ny, value);
  }

  /**
   * @brief get width
   */
  inline int size_x() const { return _nx; }

  /**
   * @brief get height
   */
  inline int size_y() const { return _ny; }

  /**
   * @brief get element (i,j) reference
   *
   * @param i width index
   * @param j height index
   */
  T &operator()(int i, int j) {
    assert(i >= 0 && i < _nx && j >= 0 && j < _ny);
    assert(i + _nx * j < _nx * _ny);
    return _array[i + _nx * j];
  }

  /**
   * @brief get const element (i,j) reference
   *
   * @param i width index
   * @param j height index
   */
  const T &operator()(int i, int j) const {
    assert(i >= 0 && i < _nx && j >= 0 && j < _ny);
    assert(i + _nx * j < _nx * _ny);
    return _array[i + _nx * j];
  }

  /**
   * @brief get clamped element (i,j) value
   *
   * @param i width index
   * @param j height index
   */
  T sample(int i, int j) const {
    i = i > 0 ? i : 0;
    i = i < _nx ? i : (_nx - 1);
    j = j > 0 ? j : 0;
    j = j < _ny ? j : (_ny - 1);
    return _array[i + _nx * j];
  }

  /**
   * @brief get clamped element (i,j) reference
   *
   * @param i width index
   * @param j height index
   */
  T &sample(int i, int j) {
    i = i > 0 ? i : 0;
    i = i < _nx ? i : (_nx - 1);
    j = j > 0 ? j : 0;
    j = j < _ny ? j : (_ny - 1);
    return _array[i + _nx * j];
  }

  /**
   * @brief fill all elements with value
   */
  void fill(T value) { std::fill(_array.begin(), _array.end(), value); }

  /**
   * @brief return a pointer such a [data, data+size] is a valid range
   */
  T *data() { return _array.data(); }
};

}  // namespace Raygen

#endif  // RAYGEN_TYPE_D2ARRAY_H_
