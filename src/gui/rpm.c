#include "rpm.h"
#include "gui/dashboard.h"
#include "state.h"
#include <stdio.h>

const char *get_rpm_value(rpm_value_t v) {
  switch (v) {
  case RPM_ZERO:
    return "0";
  case RPM_ONE:
    return "1";
  case RPM_TWO:
    return "2";
  case RPM_THREE:
    return "3";
  case RPM_FOUR:
    return "4";
  case RPM_FIVE:
    return "5";
  case RPM_SIX:
    return "6";
  case RPM_SEVEN:
    return "7";
  case RPM_EIGHT:
    return "8";
  default:
    return "N/A";
  }
}

void draw_target_idle(struct DashboardPadding padding,
                      struct ParameterCtx *ctx) {
  Vector2 rpm_gauge_bg_tl = dashboard_get_pixel_coord(padding, 0.0f, 0.0f);
  Vector2 rpm_gauge_bg_br = dashboard_get_pixel_coord(padding, 1.0f, 0.125f);
  int rpm_gauge_bg_w = rpm_gauge_bg_br.x - rpm_gauge_bg_tl.x;
  int rpm_gauge_bg_h = rpm_gauge_bg_br.y - rpm_gauge_bg_tl.y;
  int pixels_per_rpm = rpm_gauge_bg_w / MAIN_SEGMENTS;

  int x = rpm_gauge_bg_tl.x + (pixels_per_rpm * (ctx->target_idle * 0.001f));
  int y = rpm_gauge_bg_tl.y + rpm_gauge_bg_h;

  Vector2 rpm_text_size =
      MeasureTextEx(NORMAL_FONT, get_rpm_value(RPM_ZERO), TEXT_SIZE, 0.0f);
  int w = (int)roundf(pixels_per_rpm * 0.1f);
  int h = rpm_text_size.y / 3;

  Vector2 btm = (Vector2){.x = x, .y = y};
  Vector2 tl = (Vector2){.x = x - w, .y = y - h};
  Vector2 tr = (Vector2){.x = x + w, .y = y - h};

  int lower_lim = ctx->target_idle - 50;
  int upper_lim = ctx->target_idle + 50;
  DrawTriangle(tr, tl, btm,
               (ctx->rpm->target < upper_lim) && (ctx->rpm->target > lower_lim)
                   ? GREEN
                   : WHITE);
}

void draw_rpm_gauge(struct DashboardPadding padding, struct RpmCtx *ctx_rpm) {
  Vector2 rpm_text_size = MeasureTextEx(
      NORMAL_FONT_BOLD, get_rpm_value(RPM_ZERO), TEXT_SIZE_RPM, 0.0f);
  Vector2 rpm_gauge_bg_tl = dashboard_get_pixel_coord(padding, 0.0f, 0.0f);
  Vector2 rpm_gauge_bg_br = dashboard_get_pixel_coord(padding, 1.0f, 0.125f);
  int rpm_gauge_bg_w = rpm_gauge_bg_br.x - rpm_gauge_bg_tl.x;
  int rpm_gauge_bg_h = rpm_gauge_bg_br.y - rpm_gauge_bg_tl.y;
  int pixels_per_rpm = rpm_gauge_bg_w / MAIN_SEGMENTS;

  // TODO: configurable
  float redline = 6500 * 0.001;

  ctx_rpm->display = ctx_rpm->display +
                     (ctx_rpm->target - ctx_rpm->display) *
                         (1.0f - expf(-(ANIMATION_SPEED_RPM)*GetFrameTime()));
  if (fabsf(ctx_rpm->target - ctx_rpm->display) < 0.5f) {
    ctx_rpm->display = ctx_rpm->target;
  }
  float_t rpm_smol = ctx_rpm->display * 0.001f;

  // BG
  DrawRectangle(rpm_gauge_bg_tl.x, rpm_gauge_bg_tl.y, rpm_gauge_bg_w,
                rpm_gauge_bg_h, DARKGRAY);
  uint8_t alpha_r = 0;
  if ((rpm_smol >= redline - 0.5f && rpm_smol <= redline)) {
    alpha_r = (rpm_smol / 0.5f) * 128;
  } else if (rpm_smol >= redline) {
    alpha_r = 0xFF;
  } else {
    alpha_r = 0x80;
  }
  Color c_r = GetColor(0xEE000080 | alpha_r);
  DrawRectangle(rpm_gauge_bg_tl.x + (pixels_per_rpm * 6.5f), rpm_gauge_bg_tl.y,
                roundf(pixels_per_rpm * 1.5f), rpm_gauge_bg_h, c_r);

  // FG
  float redline_f = MAX_RPM * 0.001;
  DrawRectangle(rpm_gauge_bg_tl.x, rpm_gauge_bg_tl.y,
                rpm_smol >= redline_f ? pixels_per_rpm * redline_f
                                      : pixels_per_rpm * rpm_smol,
                rpm_gauge_bg_h, rpm_smol >= redline ? RED : ORANGE);

  if (false) {
    char buf[16];
    snprintf(buf, sizeof(buf), "%d", ctx_rpm->target);
    DrawTextEx(NORMAL_FONT, buf,
               (Vector2){.x = rpm_gauge_bg_tl.x + pixels_per_rpm * rpm_smol,
                         .y = rpm_gauge_bg_tl.y + (rpm_gauge_bg_h * 0.5f) -
                              (TEXT_SIZE * 0.5f)},
               TEXT_SIZE, 0, WHITE);
  }

  // rpm indicators
  for (int i = 0; i <= MAIN_SEGMENTS; i++) {
    if (i == 0) {
      int initial_x_main_segment = rpm_gauge_bg_tl.x + (pixels_per_rpm * i);
      int w = rpm_text_size.x / 2;
      DrawRectangle(initial_x_main_segment, rpm_gauge_bg_tl.y + rpm_gauge_bg_h,
                    w, rpm_text_size.y * 0.5f, WHITE);
      DrawTextEx(NORMAL_FONT_BOLD, get_rpm_value(i),
                 (Vector2){rpm_gauge_bg_tl.x + (pixels_per_rpm * i),
                           rpm_gauge_bg_tl.y + rpm_gauge_bg_h +
                               rpm_text_size.y * 0.5f},
                 TEXT_SIZE_RPM, 0, WHITE);
    } else if (i == 1) {
      uint8_t alpha = 0;
      if ((rpm_smol >= i - 0.5f && rpm_smol <= i)) {
        alpha = (rpm_smol / 0.5f) * 128;
      } else if (rpm_smol >= i) {
        alpha = 0xFF;
      } else {
        alpha = 0x80;
      }

      Color c = GetColor(0xFFFFFF80 | alpha);
      Color c_red = GetColor(0xEE000080 | alpha);

      int initial_x_main_segment = rpm_gauge_bg_tl.x + (pixels_per_rpm * i);
      int w = rpm_text_size.x / 2;
      DrawRectangle(initial_x_main_segment - (w / 2),
                    rpm_gauge_bg_tl.y + rpm_gauge_bg_h, w,
                    rpm_text_size.y * 0.5f, c);
      DrawTextEx(NORMAL_FONT_BOLD, get_rpm_value(i),
                 (Vector2){rpm_gauge_bg_tl.x + (pixels_per_rpm * i) -
                               (rpm_text_size.x * 0.5f),
                           rpm_gauge_bg_tl.y + rpm_gauge_bg_h +
                               rpm_text_size.y * 0.5f},
                 TEXT_SIZE_RPM, 0, i >= redline ? c_red : c);
    } else {
      int initial_x_main_segment = rpm_gauge_bg_tl.x + (pixels_per_rpm * i);
      int w = rpm_text_size.x / 2;

      uint8_t alpha = 0;
      if ((rpm_smol >= i - 0.5f && rpm_smol <= i)) {
        alpha = (rpm_smol / 0.5f) * 128;
      } else if (rpm_smol >= i) {
        alpha = 0xFF;
      } else {
        alpha = 0x80;
      }

      Color c = GetColor(0xFFFFFF80 | alpha);
      Color c_red = GetColor(0xEE000080 | alpha);
      if (i < redline) {
        DrawRectangle(initial_x_main_segment - (w / 2),
                      rpm_gauge_bg_tl.y + rpm_gauge_bg_h, w,
                      rpm_text_size.y * 0.5f, i >= redline ? c_red : c);
        DrawTextEx(NORMAL_FONT_BOLD, get_rpm_value(i),
                   (Vector2){rpm_gauge_bg_tl.x + (pixels_per_rpm * i) -
                                 (rpm_text_size.x * 0.5f),
                             rpm_gauge_bg_tl.y + rpm_gauge_bg_h +
                                 rpm_text_size.y * 0.5f},
                   TEXT_SIZE_RPM, 0, i >= redline ? c_red : c);
      }
    }
  }
}
