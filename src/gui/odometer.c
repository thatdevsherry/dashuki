#include "gui/dashboard.h"
#include <stdio.h>

void draw_trip_a(struct DashboardPadding padding, uint32_t v_m) {
  Vector2 center_area = dashboard_get_pixel_coord(padding, 0.0f, 0.75f);
  const char *text = "Trip";
  Vector2 center_area_adj = (Vector2){
      .x = center_area.x,
      .y = center_area.y,
  };
  DrawTextEx(NORMAL_FONT, text, center_area_adj, TEXT_SIZE, 0, WHITE);

  char buf[16];
  snprintf(buf, sizeof(buf), "%.1f km",
           (double)(floorf((float_t)v_m / 100.0f) / 10.0f));
  Vector2 value_center = dashboard_get_pixel_coord(padding, 0.0f, 0.80f);
  Vector2 value_center_adj =
      (Vector2){.x = value_center.x, .y = value_center.y};
  DrawTextEx(NORMAL_FONT_BOLD, buf, value_center_adj, TEXT_SIZE_ODO, 0, WHITE);
}

void draw_odometer(struct DashboardPadding padding, uint32_t v_m) {
  Vector2 center_area = dashboard_get_pixel_coord(padding, 0.0f, 0.90f);
  const char *text = "Odometer";
  Vector2 text_width = MeasureTextEx(NORMAL_FONT, text, TEXT_SIZE, 0);
  Vector2 center_area_adj = (Vector2){
      .x = center_area.x,
      .y = center_area.y - (text_width.y * 0.5f),
  };
  DrawTextEx(NORMAL_FONT, text, center_area_adj, TEXT_SIZE, 0, WHITE);

  char buf[16];
  snprintf(buf, sizeof(buf), "%d km", v_m / 1000);
  Vector2 value_w = MeasureTextEx(NORMAL_FONT_BOLD, buf, TEXT_SIZE_ODO, 0);
  Vector2 value_center = dashboard_get_pixel_coord(padding, 0.0f, 1.00f);
  Vector2 value_center_adj =
      (Vector2){.x = value_center.x, .y = value_center.y - (value_w.y)};
  DrawTextEx(NORMAL_FONT_BOLD, buf, value_center_adj, TEXT_SIZE_ODO, 0, WHITE);
}
