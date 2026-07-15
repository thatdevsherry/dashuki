#include "throttle.h"
#include "gui/dashboard.h"
#include "state.h"
#include <stdio.h>
#include <stdlib.h>

void draw_throttle(struct DashboardPadding padding, struct ParameterCtx *ctx) {
  Vector2 point = dashboard_get_pixel_coord(padding, 0.875f, 0.9f);
  Vector2 h_h = dashboard_get_pixel_coord(padding, 0.0f, 1.0f);
  Vector2 h_l = dashboard_get_pixel_coord(padding, 0.0f, 0.75f);
  int h = (h_h.y - h_l.y) * 0.75f;
  int w = TEXT_SIZE * 0.25f;
  DrawLine(point.x - h * 0.5f, point.y - (h * 0.5f), point.x + h * 0.5f,
           point.y - h * 0.5f, WHITE);
  ctx->throt_angle_ctx->display =
      ctx->throt_angle_ctx->display +
      (ctx->throt_angle_ctx->target - ctx->throt_angle_ctx->display) *
          (1 - expf(-20 * GetFrameTime()));
  DrawRectanglePro(
      (Rectangle){.x = point.x, .y = point.y, .width = w, .height = h},
      (Vector2){.x = w * 0.5f, .y = h * 0.5f}, ctx->throt_angle_ctx->display,
      ctx->flags->ctp ? GREEN : GOLD);
  DrawLine(point.x - h * 0.5f, point.y + (h * 0.5f), point.x + h * 0.5f,
           point.y + h * 0.5f, WHITE);

  char buf[16];
  char buf_abs[16];
  snprintf(buf, sizeof(buf), "%d°", ctx->throt_angle);
  snprintf(buf_abs, sizeof(buf_abs), "%d%%", ctx->throt_abs);
  Vector2 text_ang_w = MeasureTextEx(NORMAL_FONT, buf, TEXT_SIZE, 0);
  Vector2 text_abs_w = MeasureTextEx(NORMAL_FONT, buf_abs, TEXT_SIZE, 0);
  Vector2 center_ang = dashboard_get_pixel_coord(padding, 0.95f, 0.9f);
  Vector2 center_abs = dashboard_get_pixel_coord(padding, 0.785f, 0.9f);
  Vector2 center_ang_adj = (Vector2){
      .x = center_ang.x - (text_ang_w.x * 0.5f),
      .y = center_ang.y - (text_ang_w.y * 0.5f),
  };
  Vector2 center_abs_adj = (Vector2){
      .x = center_abs.x - (text_abs_w.x * 0.5f),
      .y = center_abs.y - (text_abs_w.y * 0.5f),
  };
  DrawTextEx(NORMAL_FONT, buf, center_ang_adj, TEXT_SIZE, 0, WHITE);
  DrawTextEx(NORMAL_FONT, buf_abs, center_abs_adj, TEXT_SIZE, 0, WHITE);

  // IACV
  Vector2 point_isc = dashboard_get_pixel_coord(padding, 0.875f, 0.75f);
  int h_isc = TEXT_SIZE * 2;
  ctx->isc->display =
      ctx->isc->display +
      (ctx->isc->target - ctx->isc->display) * (1 - expf(-10 * GetFrameTime()));
  int opening = abs((int)(((ctx->isc->display / 100.0f) * h_isc) - h_isc));
  DrawRectanglePro(
      (Rectangle){
          .x = point_isc.x, .y = point_isc.y, .width = w, .height = opening},
      (Vector2){.x = w * 0.5f, .y = h_isc * 0.5f}, 0, WHITE);

  DrawLine(point_isc.x - h_isc * 0.5f, point_isc.y - (h_isc * 0.5f),
           point_isc.x + h_isc * 0.5f, point_isc.y - (h_isc * 0.5f), WHITE);
  char buf_isc[16];
  snprintf(buf_isc, sizeof(buf_isc), "%d%%", ctx->isc->target);
  DrawTextEx(NORMAL_FONT, buf_isc,
             (Vector2){.x = point_isc.x + w * 5, .y = point_isc.y}, TEXT_SIZE,
             0, WHITE);
}
