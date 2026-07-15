#include "gui/dashboard.h"
#include <stdio.h>

void draw_speedometer(struct DashboardPadding padding, Vector2 center,
                      uint8_t v) {
  char buf[8];
  snprintf(buf, sizeof(buf), "%d", v);
  Vector2 text_w = MeasureTextEx(NORMAL_FONT_BOLD, buf, TEXT_SIZE_SPEEDO, 0);
  Vector2 center_adjusted = (Vector2){.x = center.x - (text_w.x / 2.0f),
                                      .y = center.y - (text_w.y / 2.0f)};
  DrawTextEx(NORMAL_FONT_BOLD, buf, center_adjusted, TEXT_SIZE_SPEEDO, 0,
             WHITE);

  const char *unit = "KPH";
  Vector2 text_unit_w = MeasureTextEx(NORMAL_FONT, unit, TEXT_SIZE_UNIT, 2);
  Vector2 center_unit = dashboard_get_pixel_coord(padding, 0.5f, 0.6f);
  Vector2 center_unit_adj = (Vector2){
      .x = center_unit.x - (text_unit_w.x * 0.5f),
      .y = center_unit.y - (text_unit_w.y * 0.25f),
  };
  DrawTextEx(NORMAL_FONT, unit, center_unit_adj, TEXT_SIZE_UNIT, 2, WHITE);
}
