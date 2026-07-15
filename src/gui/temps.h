#ifndef GUI_TEMPS_H
#define GUI_TEMPS_H

#include "gui/dashboard.h"
#include "state.h"

void draw_ect(struct DashboardPadding padding, struct EctCtx *ctx);
void draw_iat(struct DashboardPadding padding, int8_t v);
void draw_temp_divider(struct DashboardPadding padding);

#endif
