#ifndef MENU_H
#define MENU_H

#include <stdbool.h>

struct MenuNode {
  char *values[32];
  int len;
  int pos;
  void (*on_select)(int);
};

struct MenuLink {
  struct MenuNode nodes[10]; // no need for len, we SURELY can't have more than
                             // 10 nested menus.... righttttt?
  int curr;
};

struct MenuNode main_menu(void (*ptr)(int));
struct MenuNode view_menu(void (*ptr)(int));
struct MenuNode action_menu(void (*ptr)(int));
struct MenuNode dtc_menu(void (*ptr)(int));
struct MenuNode toggle_menu(const char *a, const char *b, void (*ptr)(int));
void init_menu(void);
void toggle_menu_visibility();
void handle_main_menu(int p);
void handle_view_menu(int p);
void handle_diag_params(int p);
void handle_avg_unit(int p);
void handle_action_menu(int p);
void handle_reset_trip(int p);
void handle_nav_back();
void handle_nav_confirm();
void handle_nav_down();
void handle_nav_up();

extern bool g_should_show_menu;
extern struct MenuLink g_menu_link;

#endif
