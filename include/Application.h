#pragma once
#ifndef RAYLIB_H
  #include "raylib.h"
#endif
#ifndef RAYGUI_H
  #include "../lib/raygui/src/raygui.h"
#endif


// #include "Component.h"
// #include <c++/v1/vector>
#include <vector>
#include <functional>

struct Window {
  Window() = default;
  Window(int w, int h) : width(w), height(h) {};
  int width = 1200;
  int height = 800;
  int display = 1;
  // bool isVisible = true; // probably not needed
};


class Application {
private:
  

public:
  Window window;
  Font font;
  Vector2 mouse;
  
  std::vector<std::function<void()>> draw_list;
  std::vector<std::function<void(Vector2)>> interaction_list;
  

  ~Application(void) = default;

  void init(void) {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);

    InitWindow(window.width, window.height, "Crocodile");
    // SetTargetFPS(60);
    font = LoadFontEx("../assets/Iosevka-Regular.ttc", 13, nullptr, 0);
    //GuiSetFont(font);
  };

  void run(void) {
    while (!WindowShouldClose()) {
      if (IsWindowResized() && !IsWindowFullscreen()) {
        window.width = GetScreenWidth();
        window.height = GetScreenHeight();
      }

  if (IsKeyPressed(KEY_ENTER) &&
          (IsKeyDown(KEY_LEFT_ALT) || IsKeyDown(KEY_RIGHT_ALT))) {
        // see what display we are on right now
        window.display = GetCurrentMonitor();

        if (IsWindowFullscreen()) {
          // if we are full screen, then go back to the windowed size
          SetWindowSize(window.width, window.height);
        } else {
          // if we are not full screen, set the window size to match the monitor
          // we are on
          SetWindowSize(GetMonitorWidth(window.display),
                        GetMonitorHeight(window.display));
        }

        // toggle the state
        ToggleFullscreen();
      }

      BeginDrawing();
      mouse = GetMousePosition();
      ClearBackground(RAYWHITE);
      

      draw();
      DrawFPS(20, 20);
      EndDrawing();

      click();
    };
    CloseWindow();
  };

  void add_draw(std::function<void()> fun) {
    draw_list.push_back(fun);
  }

  void add_click(std::function<void(Vector2)> fun) {
    interaction_list.push_back(fun);
  }

  void draw() {
    for (auto& execute : draw_list) {
      execute();
    }
  }

  void click() {
    mouse = GetMousePosition();
    for (auto& clickable : interaction_list) {
      clickable(mouse); 
    }
  }
};
