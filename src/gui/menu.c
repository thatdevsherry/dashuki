#include "src/menu.h"
#include "gui/dashboard.h"

void draw_menu() {
  struct DashboardPadding padding =
      dashboard_get_padding_pixels(DESIGN_SW, DESIGN_SH);
  Vector2 menu_tl = dashboard_get_pixel_coord(padding, 0.0f, 0.50f);
  Vector2 menu_br = dashboard_get_pixel_coord(padding, 0.25f, 1.00f);
  int w = menu_br.x - menu_tl.x;
  int h = menu_br.y - menu_tl.y;
  DrawRectangle(menu_tl.x, menu_tl.y, w, h, BLACK);
  DrawRectangleLines(menu_tl.x, menu_tl.y, w, h, WHITE);

  for (int i = 0; i < g_menu_link.nodes[g_menu_link.curr].len; i++) {
    Vector2 view_tl;
    if (i > 0) {
      Vector2 text_v = MeasureTextEx(
          NORMAL_FONT, g_menu_link.nodes[g_menu_link.curr].values[i],
          TEXT_SIZE_MENU, 0);
      view_tl = (Vector2){.x = menu_tl.x, .y = menu_tl.y + (text_v.y * i)};
    } else {
      view_tl = (Vector2){.x = menu_tl.x, .y = menu_tl.y};
    }
    DrawTextEx(NORMAL_FONT, g_menu_link.nodes[g_menu_link.curr].values[i],
               view_tl, TEXT_SIZE_ODO, 0,
               g_menu_link.nodes[g_menu_link.curr].pos == i ? WHITE : GRAY);
  }
}
