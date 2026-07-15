#ifndef GUI_DIAG_PARAMS_H
#define GUI_DIAG_PARAMS_H

#include "gui/dashboard.h"

void draw_battery(struct DashboardPadding padding, float_t bat_volt,
                  uint16_t rpm);
void draw_ign_adv(struct DashboardPadding padding, int8_t ign_adv);
void draw_load(struct DashboardPadding padding, uint8_t load);
void draw_inj(struct DashboardPadding padding, float_t inj_pw);
void draw_map(struct DashboardPadding padding, float_t map);
void draw_baro(struct DashboardPadding padding, float_t baro);

#endif
