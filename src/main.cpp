// main.cpp

#include "../lib/raylib.h"
#include "../lib/raymath.h"

struct ApplicationState {
    struct Window {
        int width = 800;
        int height = 800;
        int display = 1;
    } window;
};

typedef struct Circle {
  Vector2 center;
  int r;
} Circle;

Circle circle;
ApplicationState app;

void window_resize(void);

int main(void) {
  SetConfigFlags(FLAG_WINDOW_RESIZABLE);

  circle.center = (Vector2){400, 400};
  circle.r = 10;
  InitWindow(app.window.width, app.window.height, "Practice 2D");
  // SetTargetFPS(60);

  while (!WindowShouldClose()) {
    if (IsWindowResized() && !IsWindowFullscreen()) {
      app.window.width = GetScreenWidth();
      app.window.height = GetScreenHeight();
    }

    if (IsKeyPressed(KEY_ENTER) &&
        (IsKeyDown(KEY_LEFT_ALT) || IsKeyDown(KEY_RIGHT_ALT))) {
      // see what display we are on right now
      app.window.display = GetCurrentMonitor();

      if (IsWindowFullscreen()) {
        // if we are full screen, then go back to the windowed size
        SetWindowSize(app.window.width, app.window.height);
      } else {
        // if we are not full screen, set the window size to match the monitor
        // we are on
        SetWindowSize(GetMonitorWidth(app.window.display), GetMonitorHeight(app.window.display));
      }

      // toggle the state
      ToggleFullscreen();
    }

    BeginDrawing();
    Vector2 mouse = GetMousePosition();
    ClearBackground(RAYWHITE);
    DrawRectangleLinesEx((Rectangle){10, 10,
                                     static_cast<float>(app.window.width - 20),
                                     static_cast<float>(app.window.height - 20)},
                         1, BLACK);
    DrawCircle(circle.center.x, circle.center.y, circle.r, RED);
    DrawFPS(20, 20);
    EndDrawing();

    if (CheckCollisionPointCircle(mouse, circle.center, circle.r) &&
        IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
      circle.center.x += mouse.x - circle.center.x;
      circle.center.y += mouse.y - circle.center.y;
    }
  }

  CloseWindow();

  return 0;
}

void mouseClicked(Vector2 mouse) {}

void window_resize() {}
