#include "barrel.h"
#include "constants.h"
#include "physics.h"
#include <math.h>

void barrel_spawn(Barrel *b, Vector2 pos, float vx) {
    b->rect = (Rectangle){ pos.x, pos.y, BARREL_SIZE, BARREL_SIZE };
    b->vx = vx;
    b->vy = 0.0f;
    b->hop_time = 0.0f;
    b->roll_angle = 0.0f;
    b->on_ground = true;
    b->flying = false;
    b->active = true;
}

void barrel_spawn_arc(Barrel *b, Vector2 pos, float vx, float vy) {
    b->rect = (Rectangle){ pos.x, pos.y, BARREL_SIZE, BARREL_SIZE };
    b->vx = vx;
    b->vy = vy;
    b->hop_time = 0.0f;
    b->roll_angle = 0.0f;
    b->on_ground = false;
    b->flying = true;
    b->active = true;
}

static void barrel_bounce_off_screen(Barrel *b) {
    /* Half the time a barrel reaching the map edge bounces back instead of
     * rolling off; otherwise the off-screen check despawns it. */
    if (b->rect.x < 0.0f || b->rect.x + b->rect.width > SCREEN_WIDTH) {
        if (GetRandomValue(0, 1) == 0) {
            b->rect.x = (b->rect.x < 0.0f) ? 0.0f : SCREEN_WIDTH - b->rect.width;
            b->vx = -b->vx;
        }
    }
}

static void barrel_move_horizontal(Barrel *b, float dt) {
    b->rect.x += b->vx * dt;
    /* Spin matches the travel distance like a rolling wheel. */
    b->roll_angle += (b->vx * dt / (b->rect.width / 2.0f)) * RAD2DEG;
    if (level_solid_at_rect(b->rect)) {
        b->rect.x -= b->vx * dt;
        b->vx = -b->vx;
        if (b->on_ground) b->hop_time = BARREL_HOP_TIME;
    }
    barrel_bounce_off_screen(b);
}

static void barrel_try_take_ladder(Barrel *b, float dt) {
    if (!b->on_ground) return;
    int col = physics_tile_col(b->rect.x + b->rect.width / 2.0f, TILE_SIZE);
    int row = physics_tile_row(b->rect.y + b->rect.height + 1.0f, TILE_SIZE);
    /* Occasionally crash down a ladder instead of rolling off an edge. */
    if (level_is_ladder(col, row) && GetRandomValue(0, 1000) < (int)(dt * 300.0f)) {
        b->rect.x = col * TILE_SIZE + TILE_SIZE / 2.0f - b->rect.width / 2.0f;
        b->vx = 0.0f;
        b->on_ground = false;
    }
}

static void barrel_resolve_vertical(Barrel *b, float dt) {
    float start_y = b->rect.y;
    b->vy = physics_gravity_step(b->vy, dt);
    b->rect.y += b->vy * dt;

    b->on_ground = false;
    if (!level_solid_at_rect(b->rect)) return;
    if (b->vy > 0.0f) {
        b->rect.y = physics_snap_bottom(b->rect.y + b->rect.height, TILE_SIZE) - b->rect.height;
        b->vy = 0.0f;
        b->on_ground = true;
        /* Dropping a full step lands with a little bounce. */
        if (b->rect.y - start_y >= TILE_SIZE) b->hop_time = BARREL_HOP_TIME;
    } else if (b->vy < 0.0f) {
        b->rect.y = physics_snap_top(b->rect.y, TILE_SIZE);
        b->vy = 0.0f;
    }
}

static void barrel_flying_update(Barrel *b, float dt) {
    /* No wall collisions while airborne: the arc sails over floor edges and
     * lands on whatever platform is below it when it comes down. */
    b->vy = physics_gravity_step(b->vy, dt);
    b->rect.x += b->vx * dt;
    b->rect.y += b->vy * dt;
    barrel_bounce_off_screen(b);

    if (b->vy > 0.0f && level_solid_at_rect(b->rect)) {
        b->rect.y = physics_snap_bottom(b->rect.y + b->rect.height, TILE_SIZE) - b->rect.height;
        b->vy = 0.0f;
        b->on_ground = true;
        b->hop_time = BARREL_HOP_TIME; /* landing from a long arc bounces */
        b->flying = false; /* from here it rolls like any other barrel */
    }
    if (b->rect.x < -20.0f || b->rect.x > SCREEN_WIDTH + 20.0f || b->rect.y > SCREEN_HEIGHT + 20.0f) {
        b->active = false;
    }
}

void barrel_update(Barrel *b, float dt) {
    if (b->hop_time > 0.0f) b->hop_time -= dt;
    if (b->flying) {
        barrel_flying_update(b, dt);
        return;
    }
    barrel_move_horizontal(b, dt);
    barrel_try_take_ladder(b, dt);
    barrel_resolve_vertical(b, dt);

    if (b->rect.x < -20.0f || b->rect.x > SCREEN_WIDTH + 20.0f || b->rect.y > SCREEN_HEIGHT + 20.0f) {
        b->active = false;
    }
}