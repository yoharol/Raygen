#include <memory.h>
#include <raylib.h>
#include <raymath.h>
#include <Raygen/Camera/orbit_camera.h>
#include <Raygen/input.h>
#include <cmath>

int main(int argc, char* argv[]) {
  // Initialization
  //--------------------------------------------------------------------------------------
  int screenWidth = 1024;
  int screenHeight = 768;
  InitWindow(screenWidth, screenHeight, "orbit camera");
  SetTargetFPS(60);

  // Create background image
  //--------------------------------------------------------------------------------------
  Image img = GenImageChecked(256, 256, 64, 64, LIGHTGRAY, WHITE);
  Texture tx = LoadTextureFromImage(img);

  // setup initial camera data
  //--------------------------------------------------------------------------------------
  Camera3D orbitCam;
  orbitCam.fovy = 45;
  orbitCam.target = Vector3{0, 0, 0};
  orbitCam.position = Vector3{0, 3, 5};
  orbitCam.up = Vector3{0, 1, 0};

  Raygen::OrbitCameraControl orbit_control(orbitCam);

  while (!WindowShouldClose()) {
    // Update obit camera control
    // -----------------------------------------------------------------------------------
    Eigen::Vector2f cursor_delta_pos;
    if (IsMouseButtonDown(0) && IsKeyDown(KEY_LEFT_ALT)) {
      cursor_delta_pos = Raygen::get_mouse_delta() * GetFrameTime();
    } else {
      cursor_delta_pos = Eigen::Vector2f::Zero();
    }

    float mousewheel = GetMouseWheelMove();

    if (cursor_delta_pos.norm() + abs(mousewheel) > 0)
      orbit_control.update(orbitCam, cursor_delta_pos, mousewheel);

    // Drawing
    // -------------------------------------------------------------------------------------
    BeginDrawing();
    ClearBackground(RAYWHITE);

    BeginMode3D(orbitCam);
    // DrawPlane(Vector3{0, 0, 0}, Vector2{50, 50}, BLUE);  // simple world
    // plane

    DrawGrid(20, 1);
    DrawSphere(orbitCam.target, 0.25f, RED);
    EndMode3D();

    DrawText("Right drag to rotate, Wheel to zoom", 10, 60, 20, GREEN);

    DrawFPS(10, 10);
    EndDrawing();
    //----------------------------------------------------------------------------------
  }

  // De-Initialization
  //--------------------------------------------------------------------------------------
  CloseWindow();  // Close window and OpenGL context

  return 0;
}
