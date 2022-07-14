#include <raylib.h>
#include <raymath.h>
#include <Raygen/Camera/orbit_camera.h>
#include <Raygen/input.h>
#include <Raygen/Type/color.h>
#include <Raygen/geometry.h>
#include <iostream>
#include <vector>

int main() {
  // Initialization
  //--------------------------------------------------------------------------------------
  int screenWidth = 1024;
  int screenHeight = 768;
  InitWindow(screenWidth, screenHeight, "obj show");
  SetTargetFPS(60);

  // Initialize Camera
  //--------------------------------------------------------------------------------------
  Camera3D orbitCam;
  orbitCam.fovy = 45;
  orbitCam.target = Vector3{0, 0, 0};
  orbitCam.position = Vector3{0, 3, 5};
  orbitCam.up = Vector3{0, 1, 0};
  orbitCam.projection = CAMERA_PERSPECTIVE;
  Raygen::OrbitCameraControl orbit_control(orbitCam);

  // Load OBJ
  //--------------------------------------------------------------------------------------
  auto get_file_path = [&](std::string filepath) {
    return std::string(ASSETSPATH) + filepath;
  };
  std::vector<float> vertices;
  std::vector<unsigned short> indices;
  Raygen::load_obj(vertices, indices, get_file_path("cube.obj"));
  std::cout << "vertices: \n";
  for (int i = 0; i < vertices.size(); i++) std::cout << vertices[i] << " ";
  std::cout << "\nfaces:\n";
  for (int i = 0; i < indices.size(); i++) std::cout << indices[i] << " ";
  std::cout << "\n";

  Model model_cube = LoadModel(get_file_path("armadillo1.obj").c_str());
  Shader shader = Raygen::gen_shader();
  model_cube.materials[0].shader = shader;

  // Draw
  //--------------------------------------------------------------------------------------
  while (!WindowShouldClose()) {
    Eigen::Vector2f delta_mouse_pos = Eigen::Vector2f::Zero();
    if (IsMouseButtonDown(0) && IsKeyDown(KEY_LEFT_ALT)) {
      delta_mouse_pos = Raygen::get_mouse_delta() * GetFrameTime();
    }
    float mouse_wheel = GetMouseWheelMove();
    if (abs(mouse_wheel) + delta_mouse_pos.norm() > 0)
      orbit_control.update(orbitCam, delta_mouse_pos, mouse_wheel);

    BeginDrawing();
    ClearBackground(RAYWHITE);
    {
      BeginMode3D(orbitCam);
      DrawModel(model_cube, {0.0, 0.0, 0.0}, 1.0f, WHITE);
      DrawGrid(20, 1);
      EndMode3D();
    }
    EndDrawing();
  }
  UnloadShader(shader);
  UnloadModel(model_cube);
  CloseWindow();
}
