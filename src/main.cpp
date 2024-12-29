// main.cpp

#include "raylib.h"
//#include "raymath.h"

// #include "../lib/raygui/src/raygui.h"
#define RAYGUI_IMPLEMENTATION
//#include "raylib.h"

#include "../include/Application.h"
#include "../include/serial.h"

#include <stdio.h>
#include <dirent.h>

#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
// tty.usbmodem122391201


typedef struct Circle {
  Vector2 center;
  int r;
} Circle;

Circle circle;
Application app;

void check_for_serial_ports(void);
void draw_gui(void);
void window_resize(void);

void border();
void draw_circle();
void circle_click(Vector2);

//  GuiPanel(Rectangle bounds, const char *text); 
Serial serial;

void stop_serial() {
  if (IsKeyPressed(KEY_D)) {
    serial.disconnect();
  }
}

int main(void) {

  app.init();
  
  // SetTargetFPS(60);
  circle.center = (Vector2){400, 400};
  circle.r = 10;

  app.draw_list.push_back(border);
  app.draw_list.push_back(draw_circle);
  app.interaction_list.push_back(circle_click);
  app.draw_list.push_back(stop_serial);

  DIR *dir;
  struct dirent *entry;
  dir = opendir("/dev/");
  if (dir == NULL) {
    printf("Error opening /dev/ directory");
  }

  while ((entry = readdir(dir)) != nullptr) {
    if (strncmp(entry->d_name, "tty.", 4) == 0) {
      printf("Port: %s\n", entry->d_name);
    }
  }

  closedir(dir);
  serial.init("/dev/tty.usbmodem122391201");
  app.run();

  return 0;
}

void mouseClicked(Vector2 mouse) {}

void window_resize() {}

void border() {
    DrawRectangleLinesEx((Rectangle){10, 10,
                                     static_cast<float>(app.window.width - 20),
                                     static_cast<float>(app.window.height - 20)},
                         1, BLACK);

}
void draw_circle() {
    DrawCircle(circle.center.x, circle.center.y, circle.r, RED);
}

void circle_click(Vector2 mouse) {
    if (CheckCollisionPointCircle(mouse, circle.center, circle.r) &&
        IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
      circle.center.x += mouse.x - circle.center.x;
      circle.center.y += mouse.y - circle.center.y;
    }
}
