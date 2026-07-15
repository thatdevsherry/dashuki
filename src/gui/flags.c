#include "gui/dashboard.h"
#include "state.h"

void draw_flags(struct DashboardPadding padding, struct StatusFlags *flags) {
  if (flags->rad) {
    Vector2 center = dashboard_get_pixel_coord(padding, 0.725f, 0.5f);
    static int rad_rot = 0;
    int w = TEXT_SIZE_UNIT * 0.25f;
    int h = TEXT_SIZE_UNIT * 0.625f;
    DrawRectanglePro(
        (Rectangle){.x = center.x, .y = center.y, .width = w, .height = h},
        (Vector2){.x = w * 0.5f, .y = h * 1}, rad_rot, WHITE);
    DrawRectanglePro(
        (Rectangle){.x = center.x, .y = center.y, .width = w, .height = h},
        (Vector2){.x = w * 0.5f, .y = h * 1}, rad_rot + 120, WHITE);
    DrawRectanglePro(
        (Rectangle){.x = center.x, .y = center.y, .width = w, .height = h},
        (Vector2){.x = w * 0.5f, .y = h * 1}, rad_rot + 240, WHITE);
    rad_rot += 3;
  }

  if (flags->psp) {
    Vector2 center = dashboard_get_pixel_coord(padding, 0.5f, 0.6875f);
    int w = TEXT_SIZE_UNIT * 0.25f;
    int h = TEXT_SIZE_UNIT * 0.625f;
    Color psp_color = PURPLE;
    DrawCircleLines(center.x, center.y, h, psp_color);
    DrawRectanglePro(
        (Rectangle){.x = center.x, .y = center.y, .width = w, .height = h},
        (Vector2){.x = w * 0.5f, .y = 0}, 0, psp_color);
    DrawRectanglePro(
        (Rectangle){.x = center.x, .y = center.y, .width = w, .height = h},
        (Vector2){.x = w * 0.5f, .y = h * 1}, 90, psp_color);
    DrawRectanglePro(
        (Rectangle){.x = center.x, .y = center.y, .width = w, .height = h},
        (Vector2){.x = w * 0.5f, .y = h * 1}, 270, psp_color);
  }

  if (flags->ac) {
    Vector2 center = dashboard_get_pixel_coord(padding, 0.425f, 0.6875f);
    const char *text = "A/C";
    Vector2 w = MeasureTextEx(NORMAL_FONT, text, TEXT_SIZE, 0);
    Vector2 center_adj = {.x = center.x - (w.x * 0.5f),
                          .y = center.y - (w.y * 0.5f)};
    DrawTextEx(NORMAL_FONT, text, center_adj, TEXT_SIZE, 0, BLUE);
  }

  if (flags->el) {
    Vector2 center = dashboard_get_pixel_coord(padding, 0.575f, 0.6875f);
    const char *text = "E/L";
    Vector2 w = MeasureTextEx(NORMAL_FONT, text, TEXT_SIZE, 0);
    Vector2 center_adj = {.x = center.x - (w.x * 0.5f),
                          .y = center.y - (w.y * 0.5f)};
    DrawTextEx(NORMAL_FONT, text, center_adj, TEXT_SIZE, 0, YELLOW);
  }
}
