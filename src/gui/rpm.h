#ifndef GUI_RPM_H
#define GUI_RPM_H

#include "gui/dashboard.h"
#include "state.h"

typedef enum {
  RPM_ZERO,
  RPM_ONE,
  RPM_TWO,
  RPM_THREE,
  RPM_FOUR,
  RPM_FIVE,
  RPM_SIX,
  RPM_SEVEN,
  RPM_EIGHT,
} rpm_value_t;

const char *get_rpm_value(rpm_value_t v);
void draw_target_idle(struct DashboardPadding padding,
                      struct ParameterCtx *ctx);
void draw_rpm_gauge(struct DashboardPadding padding, struct RpmCtx *ctx_rpm);

#endif
