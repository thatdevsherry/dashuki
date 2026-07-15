#include "gui/dashboard.h"
#include "state.h"
#include <stdio.h>
#include <stdlib.h>

void draw_ect(struct DashboardPadding padding, struct EctCtx *ctx) {
  Vector2 top_left_area = dashboard_get_pixel_coord(padding, 0.825f, 0.5f);
  // BG
  DrawRing(top_left_area, TEXT_SIZE * 1.75f, TEXT_SIZE * 2.375f, 90, 270, 240,
           DARKGRAY);
  DrawRing(top_left_area, TEXT_SIZE * 1.5f, TEXT_SIZE * 1.65f, 240, 270, 240,
           RED);
  ctx->display = ctx->display +
                 (ctx->target - ctx->display) * (1 - expf(-8 * GetFrameTime()));

  if (ctx->target < 61) {
    // do nothing.
  } else if ((60 < ctx->target) && (ctx->target < 85)) {
    float_t arc_max = 90.0f;
    float_t v_max = 25.0f;
    int8_t v_normalized = ctx->target - 60;
    int16_t v_plot = (int16_t){roundf((v_normalized / v_max) * arc_max)};
    DrawRing(top_left_area, TEXT_SIZE * 1.75f, TEXT_SIZE * 2.375f, 90,
             v_plot > 0 ? v_plot + 90 : 90, 240, BLUE);
  } else if ((84 < ctx->target) && (ctx->target < 103)) {
    DrawRing(top_left_area, TEXT_SIZE * 1.75f, TEXT_SIZE * 2.375f, 90, 180, 240,
             GREEN);
  } else if ((102 < ctx->target) && (ctx->target < 110)) {
    float_t arc_max = 60.0f;
    float_t v_max = 7.0f;
    uint8_t v_normalized = abs(102 - ctx->target);
    int16_t v_plot = (int16_t){roundf((v_normalized / v_max) * arc_max)};
    DrawRing(top_left_area, TEXT_SIZE * 1.75f, TEXT_SIZE * 2.375f, 90,
             180 + v_plot, 240, YELLOW);
  } else if ((ctx->target > 109) && (ctx->target < 120)) {
    float_t arc_max = 30.0f;
    float_t v_max = 10.0f;
    uint8_t v_normalized = abs(109 - ctx->target);
    int16_t v_plot = (int16_t){roundf((v_normalized / v_max) * arc_max)};
    DrawRing(top_left_area, TEXT_SIZE * 1.75f, TEXT_SIZE * 2.375f, 90,
             240 + v_plot, 240, RED);
  } else {
    TraceLog(LOG_INFO, "WE SHOULD NEVER REACH HERE. UNHANDLED ECT VALUE");
  }

  char buf[8];
  snprintf(buf, sizeof(buf), "%d", ctx->target);
  Vector2 text_w = MeasureTextEx(NORMAL_FONT_BOLD, buf, TEXT_SIZE_TEMP, 0);
  Vector2 xy = (Vector2){.x = top_left_area.x - (text_w.x / 2),
                         .y = top_left_area.y - (text_w.y / 2)};
  DrawTextEx(NORMAL_FONT_BOLD, buf, xy, TEXT_SIZE_TEMP, 0, WHITE);

  // now for unit
  const char *unit = "°C";
  Vector2 text_w_unit = MeasureTextEx(NORMAL_FONT, unit, TEXT_SIZE_UNIT, 0);
  Vector2 xy_unit = (Vector2){.x = top_left_area.x - (text_w_unit.x * 0.5f),
                              .y = top_left_area.y + (text_w.y / 2)};
  DrawTextEx(NORMAL_FONT, unit, xy_unit, TEXT_SIZE_UNIT, 0, WHITE);
}

void draw_iat(struct DashboardPadding padding, int8_t v) {
  Vector2 top_left_area = dashboard_get_pixel_coord(padding, 0.95f, 0.5f);
  char buf[8];
  snprintf(buf, sizeof(buf), "%d", v);
  Vector2 text_w =
      MeasureTextEx(NORMAL_FONT_BOLD, buf, TEXT_SIZE_TEMP * 0.5f, 0);
  Vector2 xy = (Vector2){.x = top_left_area.x - (text_w.x / 2),
                         .y = top_left_area.y - (text_w.y / 2)};
  DrawTextEx(NORMAL_FONT_BOLD, buf, xy, TEXT_SIZE_TEMP * 0.5f, 0, WHITE);

  // now for unit
  const char *unit = "°C";
  Vector2 text_w_unit = MeasureTextEx(NORMAL_FONT, unit, TEXT_SIZE_UNIT, 0);
  Vector2 xy_unit = (Vector2){.x = top_left_area.x - (text_w_unit.x * 0.5f),
                              .y = top_left_area.y + (text_w.y / 2)};
  DrawTextEx(NORMAL_FONT, unit, xy_unit, TEXT_SIZE_UNIT, 0, WHITE);
}

void draw_temp_divider(struct DashboardPadding padding) {
  Vector2 top_area = dashboard_get_pixel_coord(padding, 0.90f, 0.375f);
  Vector2 bottom_area = dashboard_get_pixel_coord(padding, 0.90f, 0.625f);
  DrawLine(top_area.x, top_area.y, bottom_area.x, bottom_area.y, WHITE);
}
