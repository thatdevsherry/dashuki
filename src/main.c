#define _DEFAULT_SOURCE

#include "comms.h"
#include "constants.h"
#include "gui/dashboard.h"
#include "gui/guidelines.h"
#include "gui/menu.h"
#include "menu.h"
#include "raylib.h"
#include "save.h"
#include "src/save.h"
#include "state.h"
#include <pthread.h>
#include <signal.h>
#include <stdatomic.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

Font NORMAL_FONT;
Font NORMAL_FONT_BOLD;
struct ParameterCtx g_car_ctx;
pthread_mutex_t g_lock_ctx;

#ifdef DEBUG_MODE
bool debug_mode = true;
#endif

atomic_bool should_close = false;

void *save_periodic(void *arg) {
  struct ParameterCtx *ctx = arg;
  while (true) {
    if (should_close) {
      break;
    }
    sleep(15);
    pthread_mutex_lock(&g_lock_ctx);
    save_to_disk(ctx);
    pthread_mutex_unlock(&g_lock_ctx);
  }

  return 0;
}

void sigint_cb(int sig) {
  (void)sig;
  should_close = true;
}

void load_fonts(void) {
  int codepoints[256];
  int count = 0;
  for (int i = 32; i < 127; i++) {
    codepoints[count++] = i;
  }
  for (int i = 160; i < 256; i++) {
    codepoints[count++] = i;
  }
  NORMAL_FONT =
      LoadFontEx("assets/fonts/RobotoMono-Medium.ttf", 128, codepoints, count);
  NORMAL_FONT_BOLD =
      LoadFontEx("assets/fonts/RobotoMono-Bold.ttf", 128, codepoints, count);
}

int main(int argc, char **argv) {
  if (argc < 2) {
    fprintf(stderr, "Usage: ./suzuki-dashboard <DEVICE>\n");
    return -1;
  }
  signal(SIGINT, sigint_cb);

  SetConfigFlags(FLAG_WINDOW_UNDECORATED | FLAG_WINDOW_TOPMOST |
                 FLAG_MSAA_4X_HINT);
  SetTargetFPS(50);
  InitWindow(WINDOW_SW, WINDOW_SH, APP_NAME);

  load_fonts();

  SetTextureFilter(NORMAL_FONT.texture, TEXTURE_FILTER_BILINEAR);
  RenderTexture2D target = LoadRenderTexture(DESIGN_SW, DESIGN_SH);

  pthread_mutex_init(&g_lock_ctx, NULL);

#ifdef DEBUG_MODE
  Camera2D camera = {0};
  camera.target = (Vector2){DESIGN_SW / 2.0, DESIGN_SH / 2.0};
  camera.offset = (Vector2){DESIGN_SW / 2.0, DESIGN_SH / 2.0};
  camera.zoom = 1.0;
#endif

  struct RpmCtx ctx_rpm = {.target = 0, .display = 0};
  struct EctCtx ctx_ect = {.target = -40, .display = -40};
  struct StatusFlags flags = {
      .rad = true,
      .psp = true,
      .ac = true,
      .el = true,
      .ctp = false,
  };
  struct IscCtx ctx_isc = {.target = 0, .display = 0};
  struct ThrotAngle ctx_ang = {.target = 0, .display = 0};

  g_car_ctx = (struct ParameterCtx){
      .rpm = &ctx_rpm,
      .ect = &ctx_ect,
      .iat = -40,
      .isc = &ctx_isc,
      .bat_volt = 0.0f,
      .flags = &flags,
      .throt_abs = 0,
      .inj_pw = 0.0f,
      .throt_angle = 0,
      .throt_angle_ctx = &ctx_ang,
      .target_idle = 0,
      .speed = 0,
      .odometer_m = 0,
      .trip_m = 0,
      .map = 0.0f,
      .baro = 0.0f,
      .load = 0,
      .ign_adv = 0,
      .fuel_consumption_total = 0,
      .fuel_consumption_instant = 0,
      .fuel_flow_rate = 0,
      .fuel_used_total_ul = 0,
      .fuel_used_motion_ul = 0,
  };

  if (load_from_disk(&g_car_ctx) != 0) {
    TraceLog(LOG_WARNING, "Failed to read from storage file. Starting fresh");
  }
  if (load_from_disk_app_state(&g_app_settings) != 0) {
    TraceLog(LOG_WARNING, "Failed to read from storage file. Starting fresh");
  }

  init_menu();

  struct ParameterCtx ctx_local;
  memcpy(&ctx_local, &g_car_ctx, sizeof(struct ParameterCtx));
  pthread_t ecu_thread;
  pthread_t save_thread;
  struct EcuThreadArgs ecu_args = {
      .ctx = &g_car_ctx, .serial_port = argv[1], .should_close = &should_close};
  pthread_create(&ecu_thread, NULL, get_ecu_data, &ecu_args);
  pthread_create(&save_thread, NULL, save_periodic, &g_car_ctx);

  while (!should_close && !WindowShouldClose()) {

    if (IsKeyPressed(KEY_S) ||
        IsGamepadButtonPressed(0, GAMEPAD_BUTTON_MIDDLE_LEFT)) {
      toggle_menu_visibility();
    }

    // Start menu navigation handlers.
    if (g_should_show_menu) {
      if (IsKeyPressed(KEY_H) ||
          IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_FACE_RIGHT)) {
        handle_nav_back();
      } else if (IsKeyPressed(KEY_J) ||
                 IsGamepadButtonPressed(0, GAMEPAD_BUTTON_LEFT_FACE_DOWN)) {
        handle_nav_down();
      } else if (IsKeyPressed(KEY_K) ||
                 IsGamepadButtonPressed(0, GAMEPAD_BUTTON_LEFT_FACE_UP)) {
        handle_nav_up();
      } else if (IsKeyPressed(KEY_L) ||
                 IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_FACE_DOWN)) {
        handle_nav_confirm();
      }
    }

#ifdef DEBUG_MODE
    if (IsKeyPressed(KEY_D)) {
      debug_mode = !debug_mode;
    }
#endif

    if (pthread_mutex_trylock(&g_lock_ctx) == 0) {
      memcpy(&ctx_local, &g_car_ctx, sizeof(struct ParameterCtx));
      pthread_mutex_unlock(&g_lock_ctx);
    }

    BeginTextureMode(target);
#ifdef DEBUG_MODE
    camera.zoom += GetMouseWheelMove() * 0.10f;
    // pan - hold middle mouse or right mouse and drag
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
      Vector2 delta = GetMouseDelta();
      camera.target.x -= delta.x / camera.zoom;
      camera.target.y -= delta.y / camera.zoom;
    }

    // reset
    if (IsKeyPressed(KEY_ZERO)) {
      camera.zoom = 1.0f;
      camera.target = (Vector2){DESIGN_SW / 2.0, DESIGN_SH / 2.0};
    }
#endif
    ClearBackground(BLACK);

#ifdef DEBUG_MODE
    BeginMode2D(camera);
#endif

    draw_dashboard(&ctx_local);

#ifdef DEBUG_MODE
    struct DashboardPadding padding =
        dashboard_get_padding_pixels(DESIGN_SW, DESIGN_SH);
    if (debug_mode) {
      draw_guidelines(padding);
    }
    EndMode2D();
#endif

    if (g_should_show_menu) {
      draw_menu();
    }
    EndTextureMode();

    BeginDrawing();
    ClearBackground(BLACK);
    DrawTexturePro(target.texture, (Rectangle){0, 0, DESIGN_SW, -DESIGN_SH},
                   (Rectangle){0, 0, WINDOW_SW, WINDOW_SH}, (Vector2){0, 0},
                   0.0f, WHITE);
    EndDrawing();
  }
  printf("waiting for thread ecu\n");
  pthread_join(ecu_thread, NULL);
  // TODO: figure out a better way to handle save_thread join on sigint.
  // pthread_join(save_thread, NULL);
  UnloadFont(NORMAL_FONT);
  CloseWindow();
  return 0;
}
