#ifndef RAYGEN_TYPE_COLOR_H_
#define RAYGEN_TYPE_COLOR_H_

#include <raylib.h>
#include <Eigen/Core>

namespace Raygen {

class Color4f {
 public:
  Color4f() { m_rgba(3) = 1.0f; }
  explicit Color4f(unsigned int hexcolor);
  explicit Color4f(Eigen::Vector4f rgba) : m_rgba(rgba) {}
  explicit Color4f(const Color4f& copyfrom) : m_rgba(copyfrom.rgba()) {}

  inline Eigen::Vector4f rgba() const { return m_rgba; }
  inline Eigen::Vector3f rgb() const {
    return Eigen::Vector3f(m_rgba(0), m_rgba(1), m_rgba(2));
  }
  inline Eigen::Vector4i rgbai() const {
    return Eigen::Vector4i(static_cast<int>(m_rgba(0) * 255.0f),
                           static_cast<int>(m_rgba(1) * 255.0f),
                           static_cast<int>(m_rgba(2) * 255.0f),
                           static_cast<int>(m_rgba(3) * 255.0f));
  }
  inline Eigen::Vector3i rgbi() const {
    return Eigen::Vector3i(static_cast<int>(m_rgba(0) * 255.0f),
                           static_cast<int>(m_rgba(1) * 255.0f),
                           static_cast<int>(m_rgba(2) * 255.0f));
  }
  inline float* data() { return m_rgba.data(); }
  inline Color toColor() {
    const Eigen::Vector4i m_rgbai = rgbai();
    auto convert = [](int x) { return static_cast<unsigned char>(x); };
    return Color{convert(m_rgbai(0)), convert(m_rgbai(1)), convert(m_rgbai(2)),
                 convert(m_rgbai(3))};
  }

  unsigned int hex() const;
  std::string hexstr() const;

 private:
  Eigen::Vector4f m_rgba;
};  // namespace Raygen

Color4f::Color4f(unsigned int hexcolor) {
  unsigned int hc[3];
  hc[2] = hexcolor & 0xff;
  hc[1] = (hexcolor & 0xff00) >> 8;
  hc[0] = (hexcolor & 0xff0000) >> 16;
  float fc[3];
  for (int i = 0; i < 3; i++) {
    fc[i] = static_cast<float>(hc[i]) / 255.0f;
  }
  m_rgba(0) = fc[0];
  m_rgba(1) = fc[1];
  m_rgba(2) = fc[2];
  m_rgba(3) = 1.0f;
}

unsigned int Color4f::hex() const {
  Eigen::Vector3i hc = rgbi();
  unsigned int result = 0;
  for (int i = 0; i < 3; i++) {
    result <<= 8;
    result += hc(i);
  }
  return result;
}

std::string Color4f::hexstr() const {
  unsigned int result = hex();
  std::string str = "";
  for (int i = 0; i < 6; i++) {
    unsigned int tail = result & 0xf;
    result >>= 4;
    char chara;
    if (tail < 10)
      chara = '0' + tail;
    else
      chara = 'a' + (tail - 10);
    str += chara;
  }
  return str;
}

}  // namespace Raygen

#endif  // RAYGEN_TYPE_COLOR_H_
