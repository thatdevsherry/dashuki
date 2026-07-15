#ifndef GUI_DASHBOARD_H
#define GUI_DASHBOARD_H

#include "raylib.h"
#include "state.h"
#include <math.h>
#include <stddef.h>
#include <stdint.h>

// configurable
static const uint16_t MAX_RPM = 8000;
static const int MAIN_SEGMENTS = MAX_RPM / 1000.0;
static const uint8_t SUB_SEGMENTS = MAX_RPM / 100;

// window
static const int DESIGN_SW = 800;
static const int DESIGN_SH = 480;
static const float DESIGN_PADDING = 0.025f;
static const int USABLE_W = DESIGN_SW - ((DESIGN_SW * DESIGN_PADDING) * 2.0f);
static const int USABLE_H = DESIGN_SH - ((DESIGN_SH * DESIGN_PADDING) * 2.0f);

static const Color RPM_GAUGE_BG = DARKGRAY;
static const float TEXT_SIZE_MENU = DESIGN_SH * 0.05f;
static const float TEXT_SIZE = DESIGN_SH * 0.05f;
static const float TEXT_SIZE_RPM = DESIGN_SH * 0.075f;
static const float TEXT_SIZE_ODO = DESIGN_SH * 0.075f;
static const float TEXT_SIZE_TEMP = DESIGN_SH * 0.10f;
static const float TEXT_SIZE_SPEEDO = DESIGN_SH * 0.15f;
static const float TEXT_SIZE_UNIT = DESIGN_SH * 0.040f;
static const float TEXT_SIZE_OEM_LIGHTS = DESIGN_SH * 0.05f;
static const int ANIMATION_SPEED_RPM = 8;

#ifdef DEBUG_MODE
static const int WINDOW_SW = 720 * 1.5;
static const int WINDOW_SH = 576 * 1.5;
#else
static const int WINDOW_SW = 720;
static const int WINDOW_SH = 576;
#endif

struct DashboardPadding {
  int top;
  int right;
  int bottom;
  int left;
};

extern Font NORMAL_FONT;
extern Font NORMAL_FONT_BOLD;

/*
 * Get the pixel coord bounded by padding, to the provided ratio.
 *
 * For example, center x,y would be 0.5f,0.5f arg.
 */
Vector2 dashboard_get_pixel_coord(struct DashboardPadding padding, float_t x,
                                  float_t y);

void draw_dashboard(struct ParameterCtx *ctx);
/*
 * Get padding pixels of a given length, with additional logic.
 *
 * The padding pixels are checked to be even. If they come out odd,
 * it adjusts both sides to make the inner area be even number for both
 * width and height.
 */
struct DashboardPadding dashboard_get_padding_pixels(int width, int height);
int dashboard_get_usable_width(struct DashboardPadding padding);
int dashboard_get_usable_height(struct DashboardPadding padding);

// ========== For (if) whenever we decide to replace OEM cluster ==========
void draw_high_beam(struct DashboardPadding padding, bool v);
void draw_fuel_gauge(struct DashboardPadding padding, uint8_t fuel);
void draw_indicators(struct DashboardPadding padding, bool indicator_right,
                     bool indicator_left);
void draw_oem_lights(struct DashboardPadding padding, bool is_oil,
                     bool is_brake, bool is_eng, bool is_bat);
// ========================================================================

#endif
