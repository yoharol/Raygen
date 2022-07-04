#include <raylib.h>
#include <Raygen/draw.h>
#include <Raygen/Math/math.h>
#include <Raygen/Math/random.h>
#include <Raygen/Type/type.h>

int main() {
  const int width = 1024;
  const int height = 768;
  const float fwidth = static_cast<float>(width);
  const float fheight = static_cast<float>(height);
  SetConfigFlags(FLAG_WINDOW_HIGHDPI);
  SetConfigFlags(FLAG_MSAA_4X_HINT);
  InitWindow(width, height, "Mass Sprint Game");

  const int max_nodes = 100;
  const int initial_nodes = 10;
  Raygen::d2array<bool> connect(max_nodes, max_nodes, false);
  Raygen::d2array<float> dis(max_nodes, max_nodes, 0.0f);
  Raygen::d2array<float> origin_dis(max_nodes, max_nodes, 0.0f);
  int count = 0;
  std::vector<Eigen::Vector2f> pos;
  std::vector<Raygen::Color4f> col;

  Raygen::Color4f background_color = Raygen::Color4f(0xdddddd);
  Raygen::Color4f circle_color = Raygen::Color4f(0xffaa77);
  Raygen::Color4f line_color = Raygen::Color4f(0x445566);

  Image circle_icon =
      LoadImage((std::string(ASSETSPATH) + "circle-16.png").c_str());
  Texture2D circle_tex = LoadTextureFromImage(circle_icon);
  Eigen::Vector2f pos_offset(-8.0f, -8.0f);
  UnloadImage(circle_icon);

  for (int i = 0; i < initial_nodes; i++) {
    pos.emplace_back(
        Eigen::Vector2f(Raygen::random_real(fwidth * 0.3f, fwidth * 0.7f),
                        Raygen::random_real(fheight * 0.3f, fheight * 0.7f)));
    col.emplace_back(Raygen::Color4f(0x445566));
    count++;
  }

  while (!WindowShouldClose()) {
    {
      BeginDrawing();
      Raygen::clear_background(background_color);

      for (int i = 0; i < count; i++)
        for (int j = i + 1; j < count; j++) {
          Raygen::draw_line(pos[i], pos[j], line_color, 2.0f);
        }

      for (int i = 0; i < count; i++) {
        Raygen::draw_texture(circle_tex, pos[i] + pos_offset, circle_color);
      }

      EndDrawing();
    }
  }

  CloseWindow();
  return 0;
}
