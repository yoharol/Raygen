#include <raylib.h>
#include <Raygen/Math/math.h>
#include <Raygen/Math/random.h>
#include <Raygen/Type/type.h>

int main() {
  const int width = 1024;
  const int height = 768;
  InitWindow(width, height, "Mass Sprint Game");

  while (!WindowShouldClose()) {
    {
      BeginDrawing();
      ClearBackground(Raygen::Color4f(0xdddddd).toColor());
      EndDrawing();
    }
  }

  CloseWindow();
  return 0;
}
