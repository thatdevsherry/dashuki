#ifndef GUI_FUEL_H
#define GUI_FUEL_H

#include "gui/dashboard.h"
#include "state.h"

void draw_fuel_usage(struct DashboardPadding padding, struct ParameterCtx *ctx);
void draw_fuel_avg(struct DashboardPadding padding, struct ParameterCtx *ctx);

#endif
