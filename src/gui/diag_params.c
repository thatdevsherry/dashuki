#include "gui/dashboard.h"
#include <stdio.h>

void draw_load(struct DashboardPadding padding, uint8_t load) {
  char buf[8];
  const char *hdr = "LOAD:";
  snprintf(buf, sizeof(buf), "%d%%", load);
  Vector2 text_w = MeasureTextEx(NORMAL_FONT, buf, TEXT_SIZE, 0);
  Vector2 text_hdr_w = MeasureTextEx(NORMAL_FONT, hdr, TEXT_SIZE, 0);
  Vector2 center = dashboard_get_pixel_coord(padding, 0.625f, 0.95f);
  Vector2 center_adj = (Vector2){
      .x = center.x,
      .y = center.y - (text_w.y),
  };
  Vector2 center_hdr_adj = (Vector2){
      .x = center.x - (text_hdr_w.x),
      .y = center.y - (text_hdr_w.y),
  };

  DrawTextEx(NORMAL_FONT, buf, center_adj, TEXT_SIZE, 0, WHITE);
  DrawTextEx(NORMAL_FONT, hdr, center_hdr_adj, TEXT_SIZE, 0, WHITE);
}

void draw_battery(struct DashboardPadding padding, float_t bat_volt,
                  uint16_t rpm) {
  char buf[8];
  const char *hdr = "BATT:";
  snprintf(buf, sizeof(buf), "%.1fV", (double)bat_volt);
  Vector2 text_w = MeasureTextEx(NORMAL_FONT, buf, TEXT_SIZE, 0);
  Vector2 text_hdr_w = MeasureTextEx(NORMAL_FONT, hdr, TEXT_SIZE, 0);
  Vector2 center = dashboard_get_pixel_coord(padding, 0.625f, 0.85f);
  Vector2 center_adj = (Vector2){
      .x = center.x,
      .y = center.y - (text_w.y),
  };
  Vector2 center_hdr_adj = (Vector2){
      .x = center.x - (text_hdr_w.x),
      .y = center.y - (text_hdr_w.y),
  };

  Color color = WHITE;
  if ((rpm > 0 && rpm < 2051 && (bat_volt < 12.8f || bat_volt > 14.8f)) ||
      (rpm > 2050 && (bat_volt < 13.2f || bat_volt > 14.8f)) ||
      (rpm == 0 && (bat_volt < 12.2f || bat_volt > 12.8f))) {
    color = RED;
  }
  DrawTextEx(NORMAL_FONT, buf, center_adj, TEXT_SIZE, 0, color);
  DrawTextEx(NORMAL_FONT, hdr, center_hdr_adj, TEXT_SIZE, 0, color);
}

void draw_ign_adv(struct DashboardPadding padding, int8_t ign_adv) {
  char buf[16];
  const char *hdr = "ADV:";
  snprintf(buf, sizeof(buf), "%d °BTDC", ign_adv);
  Vector2 text_w = MeasureTextEx(NORMAL_FONT, buf, TEXT_SIZE, 0);
  Vector2 text_hdr_w = MeasureTextEx(NORMAL_FONT, hdr, TEXT_SIZE, 0);
  Vector2 center = dashboard_get_pixel_coord(padding, 0.375f, 0.90f);
  Vector2 center_adj = (Vector2){
      .x = center.x,
      .y = center.y - (text_w.y),
  };
  Vector2 center_hdr_adj = (Vector2){
      .x = center.x - (text_hdr_w.x),
      .y = center.y - (text_hdr_w.y),
  };

  DrawTextEx(NORMAL_FONT, buf, center_adj, TEXT_SIZE, 0, WHITE);
  DrawTextEx(NORMAL_FONT, hdr, center_hdr_adj, TEXT_SIZE, 0, WHITE);
}

void draw_inj(struct DashboardPadding padding, float_t inj_pw) {
  char buf[16];
  const char *hdr = "INJ:";
  snprintf(buf, sizeof(buf), "%.1f ms", (double)inj_pw);
  Vector2 text_w = MeasureTextEx(NORMAL_FONT, buf, TEXT_SIZE, 0);
  Vector2 text_hdr_w = MeasureTextEx(NORMAL_FONT, hdr, TEXT_SIZE, 0);
  Vector2 center = dashboard_get_pixel_coord(padding, 0.375f, 0.85f);
  Vector2 center_adj = (Vector2){
      .x = center.x,
      .y = center.y - (text_w.y),
  };
  Vector2 center_hdr_adj = (Vector2){
      .x = center.x - (text_hdr_w.x),
      .y = center.y - (text_w.y),
  };

  DrawTextEx(NORMAL_FONT, buf, center_adj, TEXT_SIZE, 0, WHITE);
  DrawTextEx(NORMAL_FONT, hdr, center_hdr_adj, TEXT_SIZE, 0, WHITE);
}

void draw_map(struct DashboardPadding padding, float_t map) {
  char buf[16];
  const char *hdr = "MAP:";
  snprintf(buf, sizeof(buf), "%.1f kPa", (double)map);
  Vector2 text_w = MeasureTextEx(NORMAL_FONT, buf, TEXT_SIZE, 0);
  Vector2 text_hdr_w = MeasureTextEx(NORMAL_FONT, hdr, TEXT_SIZE, 0);
  Vector2 center = dashboard_get_pixel_coord(padding, 0.375f, 0.95f);
  Vector2 center_adj = (Vector2){
      .x = center.x,
      .y = center.y - (text_w.y),
  };
  Vector2 center_hdr_adj = (Vector2){
      .x = center.x - (text_hdr_w.x),
      .y = center.y - (text_hdr_w.y),
  };

  DrawTextEx(NORMAL_FONT, buf, center_adj, TEXT_SIZE, 0, WHITE);
  DrawTextEx(NORMAL_FONT, hdr, center_hdr_adj, TEXT_SIZE, 0, WHITE);
}

void draw_baro(struct DashboardPadding padding, float_t baro) {
  char buf[16];
  const char *hdr = "BAR:";
  snprintf(buf, sizeof(buf), "%.1f kPa", (double)baro);
  Vector2 text_hdr_w = MeasureTextEx(NORMAL_FONT, hdr, TEXT_SIZE, 0);
  Vector2 center = dashboard_get_pixel_coord(padding, 0.375f, 0.95f);
  Vector2 center_adj = (Vector2){
      .x = center.x,
      .y = center.y,
  };
  Vector2 center_hdr_adj = (Vector2){
      .x = center.x - (text_hdr_w.x),
      .y = center.y,
  };

  DrawTextEx(NORMAL_FONT, buf, center_adj, TEXT_SIZE, 0, WHITE);
  DrawTextEx(NORMAL_FONT, hdr, center_hdr_adj, TEXT_SIZE, 0, WHITE);
}
