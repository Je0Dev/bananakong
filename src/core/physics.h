#ifndef PHYSICS_H
#define PHYSICS_H

#include "raylib.h"
#include "constants.h"

/* Apply gravity to a vertical velocity for one frame. */
float physics_gravity_step(float vy, float dt);

/* Snap a body's bottom edge down onto the platform row below it. */
float physics_snap_bottom(float bottom, float tile);

/* Snap a body's top edge up against the platform row above it. */
float physics_snap_top(float top, float tile);

/* Convert a pixel x coordinate into a grid column index. */
int physics_tile_col(float x, float tile);

/* Convert a pixel y coordinate into a grid row index. */
int physics_tile_row(float y, float tile);

/* True if two axis-aligned rectangles overlap (strict edge touching). */
bool physics_aabb_overlap(Rectangle a, Rectangle b);

#endif