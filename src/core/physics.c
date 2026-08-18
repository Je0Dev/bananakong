#include "physics.h"

float physics_gravity_step(float vy, float dt) {
    return vy + GRAVITY * dt;
}

float physics_snap_bottom(float bottom, float tile) {
    return (float)((int)(bottom / tile)) * tile;
}

float physics_snap_top(float top, float tile) {
    return (float)((int)(top / tile) + 1) * tile;
}

int physics_tile_col(float x, float tile) {
    return (int)(x / tile);
}

int physics_tile_row(float y, float tile) {
    return (int)(y / tile);
}

bool physics_aabb_overlap(Rectangle a, Rectangle b) {
    return a.x < b.x + b.width &&
           a.x + a.width > b.x &&
           a.y < b.y + b.height &&
           a.y + a.height > b.y;
}