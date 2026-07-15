#include "gui/dashboard.h"
#include "raylib.h"

void draw_guidelines(struct DashboardPadding padding) {
  // screen edges
  {
    DrawLine(0, 0, DESIGN_SW, 0, RED);
    DrawLine(0, DESIGN_SH, DESIGN_SW, DESIGN_SH, RED);
    DrawLine(0, 0, 0, DESIGN_SH, RED);
    DrawLine(DESIGN_SW, 0, DESIGN_SW, DESIGN_SH, RED);
  }

  // cross-hair
  {
    int center_x = dashboard_get_usable_width(padding) / 2;
    int center_y = dashboard_get_usable_height(padding) / 2;
    DrawLine(padding.left + center_x, padding.top, padding.left + center_x,
             padding.bottom, WHITE);
    DrawLine(padding.left, padding.top + center_y, padding.right,
             padding.top + center_y, WHITE);
  }

  //  25/75
  {
    //  Vertical bars
    int two_fourth_v = (padding.right - padding.left) * 0.25f;
    int three_fourth_v = (padding.right - padding.left) * 0.75f;
    DrawLine(two_fourth_v + padding.left, padding.top,
             two_fourth_v + padding.left, padding.bottom, WHITE);
    DrawLine(three_fourth_v + padding.left, padding.top,
             three_fourth_v + padding.left, padding.bottom, WHITE);

    //  Horizontal bars
    int two_fourth_h = (padding.bottom - padding.top) * 0.25f;
    int three_fourth_h = (padding.bottom - padding.top) * 0.75f;
    DrawLine(padding.left, two_fourth_h + padding.top, padding.right,
             two_fourth_h + padding.top, WHITE);
    DrawLine(padding.left, three_fourth_h + padding.top, padding.right,
             three_fourth_h + padding.top, WHITE);
  }

  // Padding
  {
    DrawLine(padding.left, padding.top, padding.right, padding.top, WHITE);
    DrawLine(padding.left, padding.bottom, padding.right, padding.bottom,
             WHITE);
    DrawLine(padding.left, padding.top, padding.left, padding.bottom, WHITE);
    DrawLine(padding.right, padding.top, padding.right, padding.bottom, WHITE);
  }
}
