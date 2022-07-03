#include <Raygen/Type/color.h>
#include <Raygen/Math/random.h>
#include <iostream>

int main() {
  bool all_pass = true;
  for (int i = 0; i < 1000; i++) {
    unsigned int random_color_hex =
        static_cast<unsigned int>(Raygen::random_real<double>(0, 0xffffff));
    Raygen::Color4f color(random_color_hex);
    unsigned int color_hex = color.hex();
    if (color_hex != random_color_hex) {
      std::cout << "Error at input " << random_color_hex << " but output "
                << color_hex << std::endl;
      all_pass = false;
    }
  }
  if (all_pass) std::cout << "pass" << std::endl;
}
