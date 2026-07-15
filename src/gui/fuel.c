#include "gui/dashboard.h"
#include "state.h"
#include <stdio.h>

/*
 * Fuel consumption instant
 */
void draw_fuel_usage(struct DashboardPadding padding,
                     struct ParameterCtx *ctx) {
  Vector2 center_area = dashboard_get_pixel_coord(padding, 0.0f, 0.5f);
  const char *text = "Fuel used (L)";
  Vector2 center_area_adj = (Vector2){
      .x = center_area.x,
      .y = center_area.y,
  };
  DrawTextEx(NORMAL_FONT, text, center_area_adj, TEXT_SIZE, 0, WHITE);

  char buf[16];
  snprintf(buf, sizeof(buf), "%.1f (%.1f)",
           (double)(ctx->fuel_used_total_ul / 1000000.0f), ctx->fuel_flow_rate);
  Vector2 value_center = dashboard_get_pixel_coord(padding, 0.0f, 0.55f);
  Vector2 value_center_adj =
      (Vector2){.x = value_center.x, .y = value_center.y};
  DrawTextEx(NORMAL_FONT_BOLD, buf, value_center_adj, TEXT_SIZE_ODO, 0, WHITE);
}

void draw_fuel_avg(struct DashboardPadding padding, struct ParameterCtx *ctx) {
  Vector2 center_area = dashboard_get_pixel_coord(padding, 0.0f, 0.65f);
  const char *text;
  if (g_app_settings.fuel_avg_unit == FUEL_AVG_UNIT_KMPL) {
    text = "Avg (km/L)";
  } else {
    text = "Avg (L/100km)";
  }
  Vector2 text_width = MeasureTextEx(NORMAL_FONT, text, TEXT_SIZE, 0);
  Vector2 center_area_adj = (Vector2){
      .x = center_area.x,
      .y = center_area.y - (text_width.y * 0.5f),
  };
  DrawTextEx(NORMAL_FONT, text, center_area_adj, TEXT_SIZE, 0, WHITE);

  char buf[16];
  snprintf(buf, sizeof(buf), "%.1f (%.1f)", ctx->fuel_consumption_total,
           (double)ctx->fuel_consumption_instant);
  Vector2 value_center = dashboard_get_pixel_coord(padding, 0.0f, 0.675f);
  Vector2 value_center_adj =
      (Vector2){.x = value_center.x, .y = value_center.y};
  DrawTextEx(NORMAL_FONT_BOLD, buf, value_center_adj, TEXT_SIZE_ODO, 0, WHITE);
}
