#ifndef UI_DRAW_H
#define UI_DRAW_H

#include "raylib.h"

/* Draw a Kenney UI panel as a 9-slice: corners and border stay at native
 * pixel size while the interior stretches, so panels look crisp at any size. */
void ui_draw_panel(Texture2D tex, Rectangle dest);

#endif