#include "menu.h"
#include "raylib.h"
#include "save.h"
#include "state.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

bool g_should_show_menu = false;
struct MenuLink g_menu_link;

void init_menu(void) {
  g_menu_link =
      (struct MenuLink){.nodes = {main_menu(handle_main_menu)}, .curr = 0};
}

void toggle_menu_visibility() {
  g_should_show_menu = !g_should_show_menu;
  g_menu_link.curr = 0;
  g_menu_link.nodes[0].pos = 0;
  save_to_disk_app_state(&g_app_settings);
}

void handle_main_menu(int p) {
  switch (p) {
  case 0:
    g_menu_link.nodes[++g_menu_link.curr] = view_menu(handle_view_menu);
    break;
  case 1:
    g_menu_link.nodes[++g_menu_link.curr] = action_menu(handle_action_menu);
    break;
  default:
    TraceLog(LOG_ERROR, "Reached outside switch cases. int=%d", p);
    exit(1);
  }
}

void handle_action_menu(int p) {
  switch (p) {
  case 0: // reset trip
    g_menu_link.nodes[++g_menu_link.curr] =
        toggle_menu("NO", "YES", handle_reset_trip);
    break;
  default:
    TraceLog(LOG_ERROR, "Reached outside switch cases. int=%d", p);
    exit(1);
  }
}

void handle_reset_trip(int p) {
  switch (p) {
  case true:
    pthread_mutex_lock(&g_lock_ctx);
    g_car_ctx.trip_m = 0;
    g_car_ctx.fuel_used_total_ul = 0;
    g_car_ctx.fuel_used_motion_ul = 0;
    save_to_disk(&g_car_ctx);
    pthread_mutex_unlock(&g_lock_ctx);
    __attribute((fallthrough));
  case false:
    g_menu_link.curr--;
    break;
  default:
    TraceLog(LOG_ERROR, "Reached outside switch cases. int=%d", p);
    exit(1);
    break;
  }
}

void handle_view_menu(int p) {
  switch (p) {
  case 0: // diag params
    g_menu_link.nodes[++g_menu_link.curr] =
        toggle_menu("OFF", "ON", handle_diag_params);
    break;
  case 1:
    g_menu_link.nodes[++g_menu_link.curr] =
        toggle_menu("KMPL", "LP100KM", handle_avg_unit);
    break;
  default:
    TraceLog(LOG_ERROR, "Reached outside switch cases. int=%d", p);
    exit(1);
  }
}

void handle_avg_unit(int p) {
  switch (p) {
  case 0:
    g_app_settings.fuel_avg_unit = FUEL_AVG_UNIT_KMPL;
    g_menu_link.curr--;
    break;
  case 1:
    g_app_settings.fuel_avg_unit = FUEL_AVG_UNIT_LP100KM;
    g_menu_link.curr--;
    break;
  default:
    TraceLog(LOG_ERROR, "Reached outside switch cases. int=%d", p);
    exit(1);
    break;
  }
}

void handle_diag_params(int p) {
  switch (p) {
  case false:
    g_app_settings.show_diag_params = p;
    g_menu_link.curr--;
    break;
  case true:
    g_app_settings.show_diag_params = p;
    g_menu_link.curr--;
    break;
  default:
    TraceLog(LOG_ERROR, "Reached outside switch cases. int=%d", p);
    exit(1);
    break;
  }
}

struct MenuNode main_menu(void (*ptr)(int)) {
  const char *values[] = {"View", "Action"};
  size_t len = sizeof(values) / sizeof(values[0]);
  struct MenuNode main_menu = {.pos = 0, .len = len, .on_select = ptr};
  memcpy(main_menu.values, values, sizeof(values));
  return main_menu;
}

struct MenuNode view_menu(void (*ptr)(int)) {
  const char *values[] = {"Diag params", "avg unit"};
  size_t len = sizeof(values) / sizeof(values[0]);
  struct MenuNode menu_node = {.pos = 0, .len = len, .on_select = ptr};
  memcpy(menu_node.values, values, sizeof(values));
  return menu_node;
}

struct MenuNode action_menu(void (*ptr)(int)) {
  const char *values[] = {"Reset trip"};
  size_t len = sizeof(values) / sizeof(values[0]);
  struct MenuNode menu_node = {.pos = 0, .len = len, .on_select = ptr};
  memcpy(menu_node.values, values, sizeof(values));
  return menu_node;
}

struct MenuNode toggle_menu(const char *a, const char *b, void (*ptr)(int)) {
  const char *values[] = {a, b};
  size_t len = sizeof(values) / sizeof(values[0]);
  struct MenuNode menu_node = {.pos = 0, .len = len, .on_select = ptr};
  memcpy(menu_node.values, values, sizeof(values));
  return menu_node;
}

void handle_nav_back() {
  if (g_menu_link.curr == 0) {
    g_menu_link.nodes[0].pos = 0;
    g_should_show_menu = false;
  } else {
    g_menu_link.curr--;
  }
}
void handle_nav_confirm() {
  g_menu_link.nodes[g_menu_link.curr].on_select(
      g_menu_link.nodes[g_menu_link.curr].pos);
}
void handle_nav_down() {
  if (g_menu_link.nodes[g_menu_link.curr].pos <
      g_menu_link.nodes[g_menu_link.curr].len - 1) {
    g_menu_link.nodes[g_menu_link.curr].pos++;
  }
}
void handle_nav_up() {
  if (g_menu_link.nodes[g_menu_link.curr].pos > 0) {
    g_menu_link.nodes[g_menu_link.curr].pos--;
  }
}
