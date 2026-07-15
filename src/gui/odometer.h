#ifndef GUI_ODOMETER_H
#define GUI_ODOMETER_H

#include "gui/dashboard.h"

void draw_odometer(struct DashboardPadding padding, uint32_t v_m);
void draw_trip_a(struct DashboardPadding padding, uint32_t v_m);

#endif
