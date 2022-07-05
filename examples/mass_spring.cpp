#include <raylib.h>
#include <Raygen/draw.h>
#include <Raygen/Math/math.h>
#include <Raygen/Math/random.h>
#include <Raygen/Type/type.h>
#include <Raygen/file.h>

/**
 * Global Variables
 */
const int max_nodes = 100;
int count = 0;
const int initial_nodes = 10;
const float origin_dis = 0.15f;
const float detect_dis = 0.2f;
const float gravity = -9.8f;
const float mass = 1.0f;
const float stiffness = 3000.0f;
const float damping = 20.0f;

/**
 * @brief Add new node
 */
void add_node(Eigen::Vector2f new_pos, std::vector<Eigen::Vector2f>& pos,
              std::vector<Eigen::Vector2f>& vel,
              Raygen::d2array<bool>& connect) {
  pos.emplace_back(new_pos);
  vel.emplace_back(Eigen::Vector2f::Zero());
  for (int i = 0; i < count; i++) {
    const Eigen::Vector2f& node_pos = pos[i];
    float distance = (node_pos - new_pos).norm();
    if (distance < detect_dis) {
      connect.set(i, count, true);
      connect.set(count, i, true);
    }
  }
  count++;
}

/**
 * @brief Update in each time frame
 */
void update(const float delta_t, std::vector<Eigen::Vector2f>& pos,
            std::vector<Eigen::Vector2f>& vel, Raygen::d2array<bool>& connect) {
  for (int i = 0; i < count; i++) {
    Eigen::Vector2f force = Eigen::Vector2f(0.0f, 1.0f) * gravity * mass;
    for (int j = 0; j < count; j++) {
      if (connect.get(i, j)) {
        Eigen::Vector2f dir = pos[i] - pos[j];
        float distance = dir.norm();
        dir /= distance;
        force += dir * stiffness * (origin_dis - distance);
      }
    }
    vel[i] += delta_t * force / mass;
    vel[i] *= exp(-delta_t * damping);
  }

  for (int i = 0; i < count; i++) {
    pos[i] += vel[i] * delta_t;
    if (pos[i](1) < 0) {
      pos[i](1) = 0.0f;
      vel[i](1) = 0.0f;
    }
  }
}

int main() {
  const int width = 1024;
  const int height = 768;
  const float fwidth = static_cast<float>(width);
  const float fheight = static_cast<float>(height);
  SetConfigFlags(FLAG_WINDOW_HIGHDPI);
  SetConfigFlags(FLAG_MSAA_4X_HINT);
  SetTargetFPS(60);
  InitWindow(width, height, "Mass Spring Game");

  Raygen::d2array<bool> connect(max_nodes, max_nodes, false);
  std::vector<Eigen::Vector2f> pos;
  std::vector<Eigen::Vector2f> vel;

  Raygen::Color4f background_color(0xdddddd);
  Raygen::Color4f circle_color(0xffaa77);
  Raygen::Color4f line_color(0x445566);

  Texture2D circle_tex =
      Raygen::load_texture2d(std::string(ASSETSPATH) + "circle-16.png");
  Eigen::Vector2f pos_offset(-8.0f / fwidth, 8.0f / fheight);

  for (int i = 0; i < initial_nodes; i++) {
    Eigen::Vector2f new_pos = Eigen::Vector2f(Raygen::random_real(0.3f, 0.7f),
                                              Raygen::random_real(0.3f, 0.7f));
    add_node(new_pos, pos, vel, connect);
  }

  while (!WindowShouldClose()) {
    {
      if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        Eigen::Vector2f mouse_pos = Raygen::get_mouse_position();
        add_node(mouse_pos, pos, vel, connect);
      }
      update(0.01f, pos, vel, connect);
      BeginDrawing();
      Raygen::clear_background(background_color);
      for (int i = 0; i < count; i++)
        for (int j = i + 1; j < count; j++) {
          if (connect.get(i, j))
            Raygen::draw_line(pos[i], pos[j], line_color, 4.0f, fwidth,
                              fheight);
        }
      for (int i = 0; i < count; i++) {
        Raygen::draw_texture(circle_tex, pos[i] + pos_offset, circle_color,
                             fwidth, fheight);
      }
      DrawFPS(100, 100);
      EndDrawing();
    }
  }
  CloseWindow();
  return 0;
}
