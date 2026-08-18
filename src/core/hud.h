#ifndef HUD_H
#define HUD_H

#include "game.h"

/* Draw the in-game HUD: lives, live score (flashing on change), the level
 * badge, and a climb meter showing progress toward the goal. */
void hud_draw(const Game *g);

#endif