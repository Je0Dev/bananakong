#ifndef LEVEL_GEN_H
#define LEVEL_GEN_H

#include "constants.h"

/* Fill a level grid buffer with a deterministic procedural layout. Rows must
 * have GRID_COLS+1 columns so `level_row` can NUL-terminate them. Layout: a
 * full-width ground, Kong's platform (row 6, cols 0..25) holding the goal,
 * and 1..4 intermediate platforms that widen as they descend; ladders connect
 * solid-to-solid so nothing floats. */
void level_gen_fill(unsigned seed, int level_index, char grid[GRID_ROWS][GRID_COLS + 1]);

/* Number of intermediate platforms for a level (1..4, growing with index). */
int level_gen_floors(int level_index);

/* The seed for the level after the one generated with `seed`. */
unsigned level_gen_next_seed(unsigned seed);

#endif