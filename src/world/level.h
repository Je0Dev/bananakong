#ifndef LEVEL_H
#define LEVEL_H

#include "raylib.h"
#include "constants.h"
#include "assets.h"

/* Layout metadata: player spawn points and the golden flag goal tile. */
typedef struct Level {
    Vector2 spawn_left;   /* player starts bottom-left */
    Vector2 spawn_right;  /* alternate spawn, bottom-right */
    Vector2 goal;
} Level;

/* Fill a Level with spawn/goal positions for a procedurally generated grid.
 * `seed` selects the layout, `level_index` scales the number of platforms. */
void level_init(Level *level, int level_index, unsigned seed);

/* Character at a grid tile; '.' for anything outside the grid. */
char level_tile_at(int col, int row);

/* Raw grid row string (GRID_COLS chars) for layout validation; NULL if OOB. */
const char *level_row(int row);

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