#include "gui/dashboard.h"
#include "gui/diag_params.h"
#include "gui/flags.h"
#include "gui/fuel.h"
#include "gui/odometer.h"
#include "gui/rpm.h"
#include "gui/speed.h"
#include "gui/temps.h"
#include "gui/throttle.h"
#include "raylib.h"
#include <math.h>
#include <stdint.h>

void draw_dashboard(struct ParameterCtx *ctx) {
  struct DashboardPadding padding =
      dashboard_get_padding_pixels(DESIGN_SW, DESIGN_SH);
  draw_rpm_gauge(padding, ctx->rpm);
  draw_ect(padding, ctx->ect);
  draw_flags(padding, ctx->flags);
  draw_throttle(padding, ctx);
  draw_iat(padding, ctx->iat);
  draw_temp_divider(padding);
  draw_odometer(padding, ctx->odometer_m);
  draw_trip_a(padding, ctx->trip_m);
  draw_fuel_usage(padding, ctx);
  draw_fuel_avg(padding, ctx);
  Vector2 speedo_center = dashboard_get_pixel_coord(padding, 0.5f, 0.5f);
  draw_speedometer(padding, speedo_center, ctx->speed);
  draw_target_idle(padding, ctx);

  if (g_app_settings.show_diag_params) {
    draw_battery(padding, ctx->bat_volt, ctx->rpm->target);
    draw_load(padding, ctx->load);
    draw_map(padding, ctx->map);
    draw_baro(padding, ctx->baro);
    draw_inj(padding, ctx->inj_pw);
    draw_ign_adv(padding, ctx->ign_adv);
  }
}

void draw_high_beam(struct DashboardPadding padding, bool v) {
  Vector2 center_area = dashboard_get_pixel_coord(padding, 0.125f, 0.5f);
  const char *text = "HI";
  Vector2 text_w = MeasureTextEx(NORMAL_FONT, text, TEXT_SIZE, 0);
  Vector2 xy = (Vector2){.x = center_area.x - (text_w.x * 0.5f),
                         .y = center_area.y - (text_w.y * 0.5f)};
  if (v) {
    DrawTextEx(NORMAL_FONT, text, xy, TEXT_SIZE, 0, BLUE);
  }
}

void draw_fuel_gauge(struct DashboardPadding padding, uint8_t fuel) {
  Vector2 center_area = dashboard_get_pixel_coord(padding, 0.875f, 0.875f);
  DrawRing(center_area, TEXT_SIZE * 1.75f, TEXT_SIZE * 2.375f, 90, 360, 240,
           DARKGRAY);
  DrawRing(center_area, TEXT_SIZE * 1.5f, TEXT_SIZE * 1.65f, 90, 120, 240, RED);
  float_t arc_max = 270.0f;
  uint16_t to_draw = (uint16_t)roundf(((float_t)fuel / 255.0f) * arc_max);
  DrawRing(center_area, TEXT_SIZE * 1.75f, TEXT_SIZE * 2.375f, 90, 90 + to_draw,
           240, 90 + to_draw <= 120 ? RED : ORANGE);
}

void draw_oem_lights(struct DashboardPadding padding, bool is_oil,
                     bool is_brake, bool is_eng, bool is_bat) {
  Vector2 center_area_oil = dashboard_get_pixel_coord(padding, 0.45f, 0.85f);
  Vector2 center_area_bat = dashboard_get_pixel_coord(padding, 0.55f, 0.85f);
  Vector2 center_area_brake = dashboard_get_pixel_coord(padding, 0.5f, 0.75f);
  Vector2 center_area_eng = dashboard_get_pixel_coord(padding, 0.5f, 0.95f);
  const char *bat = "BAT";
  const char *oil = "OIL";
  const char *eng = "CHECK ENGINE";
  const char *brake = "BRAKE";
  Vector2 text_unit_w_bat =
      MeasureTextEx(NORMAL_FONT, bat, TEXT_SIZE_OEM_LIGHTS, 0);
  Vector2 text_unit_w_oil =
      MeasureTextEx(NORMAL_FONT, oil, TEXT_SIZE_OEM_LIGHTS, 0);
  Vector2 text_unit_w_eng =
      MeasureTextEx(NORMAL_FONT, eng, TEXT_SIZE_OEM_LIGHTS, 0);
  Vector2 text_unit_w_brake =
      MeasureTextEx(NORMAL_FONT, brake, TEXT_SIZE_OEM_LIGHTS, 0);
  Vector2 center_area_bat_adj =
      (Vector2){.x = center_area_bat.x - (text_unit_w_bat.x * 0.5f),
                .y = center_area_bat.y - (text_unit_w_bat.y * 0.5f)};
  Vector2 center_area_oil_adj =
      (Vector2){.x = center_area_oil.x - (text_unit_w_oil.x * 0.5f),
                .y = center_area_oil.y - (text_unit_w_oil.y * 0.5f)};
  Vector2 center_area_eng_adj =
      (Vector2){.x = center_area_eng.x - (text_unit_w_eng.x * 0.5f),
                .y = center_area_eng.y - (text_unit_w_eng.y * 0.5f)};
  Vector2 center_area_brake_adj =
      (Vector2){.x = center_area_brake.x - (text_unit_w_brake.x * 0.5f),
                .y = center_area_brake.y - (text_unit_w_brake.y * 0.5f)};
  if (is_bat)
    DrawTextEx(NORMAL_FONT, bat, center_area_bat_adj, TEXT_SIZE_OEM_LIGHTS, 0,
               RED);
  if (is_oil)
    DrawTextEx(NORMAL_FONT, oil, center_area_oil_adj, TEXT_SIZE_OEM_LIGHTS, 0,
               RED);
  if (is_brake)
    DrawTextEx(NORMAL_FONT, brake, center_area_brake_adj, TEXT_SIZE_OEM_LIGHTS,
               0, RED);
  if (is_eng)
    DrawTextEx(NORMAL_FONT, eng, center_area_eng_adj, TEXT_SIZE_OEM_LIGHTS, 0,
               YELLOW);
}

void draw_indicators(struct DashboardPadding padding, bool indicator_right,
                     bool indicator_left) {
  if (indicator_right) {
    Vector2 t_top = dashboard_get_pixel_coord(padding, 0.68f, 0.46f);
    Vector2 t_bottom = dashboard_get_pixel_coord(padding, 0.68f, 0.54f);
    Vector2 t_right = dashboard_get_pixel_coord(padding, 0.72f, 0.50f);
    Vector2 r_pos = dashboard_get_pixel_coord(padding, 0.65f, 0.5f);
    int w = dashboard_get_pixel_coord(padding, 0.68f, 0.5f).x - r_pos.x;
    int h = dashboard_get_pixel_coord(padding, 0.68f, 0.55f).y - r_pos.y;
    DrawTriangle(t_top, t_bottom, t_right, indicator_right ? GREEN : DARKGRAY);
    DrawRectangle(r_pos.x, r_pos.y - (h * 0.5f), w, h,
                  indicator_right ? GREEN : DARKGRAY);
  }

  if (indicator_left) {
    Vector2 t_top = dashboard_get_pixel_coord(padding, 0.32f, 0.46f);
    Vector2 t_left = dashboard_get_pixel_coord(padding, 0.28f, 0.50f);
    Vector2 t_bottom = dashboard_get_pixel_coord(padding, 0.32f, 0.54f);
    Vector2 r_pos = dashboard_get_pixel_coord(padding, 0.32f, 0.5f);
    int w = dashboard_get_pixel_coord(padding, 0.35f, 0.5f).x - r_pos.x;
    int h = dashboard_get_pixel_coord(padding, 0.35f, 0.55f).y - r_pos.y;
    DrawTriangle(t_top, t_left, t_bottom, indicator_left ? GREEN : DARKGRAY);
    DrawRectangle(r_pos.x, r_pos.y - (h * 0.5f), w, h,
                  indicator_left ? GREEN : DARKGRAY);
  }
}

Vector2 dashboard_get_pixel_coord(struct DashboardPadding padding, float_t x,
                                  float_t y) {
  int x_before_calc = dashboard_get_usable_width(padding) * x;
  int x_after = padding.left + x_before_calc;

  int y_before_calc = dashboard_get_usable_height(padding) * y;
  int y_after = padding.top + y_before_calc;

  return (Vector2){x = x_after, y = y_after};
}

struct DashboardPadding dashboard_get_padding_pixels(int width, int height) {
  int left_padding = 0 + (width * DESIGN_PADDING);
  int right_padding = width - (int)(width * DESIGN_PADDING);

  int top_padding = 0 + (height * DESIGN_PADDING);
  int bottom_padding = height - (int)(height * DESIGN_PADDING);

  left_padding = (left_padding % 2 == 0) ? left_padding : left_padding - 1;
  right_padding = (right_padding % 2 == 0) ? right_padding : right_padding + 1;

  top_padding = (top_padding % 2 == 0) ? top_padding : top_padding - 1;
  bottom_padding =
      (bottom_padding % 2 == 0) ? bottom_padding : bottom_padding + 1;

  return (struct DashboardPadding){
      .top = top_padding,
      .bottom = bottom_padding,
      .right = right_padding,
      .left = left_padding,
  };
}

int dashboard_get_usable_width(struct DashboardPadding padding) {
  return padding.right - padding.left;
}
int dashboard_get_usable_height(struct DashboardPadding padding) {
  return padding.bottom - padding.top;
}
