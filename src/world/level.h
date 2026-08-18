#ifndef LEVEL_H
#define LEVEL_H

#include "raylib.h"
#include "constants.h"
#include "assets.h"

/* Layout metadata: player spawn point and the golden flag goal tile. */
typedef struct Level {
    Vector2 spawn;
    Vector2 goal;
} Level;

/* Fill a Level with spawn/goal positions for the fixed grid map. */
void level_init(Level *level);

/* Character at a grid tile; '.' for anything outside the grid. */
char level_tile_at(int col, int row);

/* True if the tile is solid (platform '#' or goal 'G'). */
bool level_is_solid(int col, int row);

/* True if the tile is a climbable ladder 'L'. */
bool level_is_ladder(int col, int row);

/* True if the tile is the win goal 'G'. */
bool level_is_goal(int col, int row);

/* True if any solid tile overlaps the given rectangle. */
bool level_solid_at_rect(Rectangle r);

/* True if any ladder tile overlaps the given rectangle. */
bool level_ladder_at_rect(Rectangle r);

/* Render the background and every tile of the level grid. */
void level_draw(const Assets *a);

#endif