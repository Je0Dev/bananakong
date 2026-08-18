#include "barrel.h"
#include "constants.h"
#include "physics.h"

void barrel_spawn(Barrel *b, Vector2 pos, float vx) {
    b->rect = (Rectangle){ pos.x, pos.y, BARREL_SIZE, BARREL_SIZE };
    b->vx = vx;
    b->vy = 0.0f;
    b->on_ground = true;
    b->active = true;
}

static void barrel_move_horizontal(Barrel *b, float dt) {
    b->rect.x += b->vx * dt;
    if (level_solid_at_rect(b->rect)) {
        b->rect.x -= b->vx * dt;
        b->vx = -b->vx;
    }
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
    b->vy = physics_gravity_step(b->vy, dt);
    b->rect.y += b->vy * dt;

    b->on_ground = false;
    if (!level_solid_at_rect(b->rect)) return;
    if (b->vy > 0.0f) {
        b->rect.y = physics_snap_bottom(b->rect.y + b->rect.height, TILE_SIZE) - b->rect.height;
        b->vy = 0.0f;
        b->on_ground = true;
    } else if (b->vy < 0.0f) {
        b->rect.y = physics_snap_top(b->rect.y, TILE_SIZE);
        b->vy = 0.0f;
    }
}

void barrel_update(Barrel *b, float dt) {
    barrel_move_horizontal(b, dt);
    barrel_try_take_ladder(b, dt);
    barrel_resolve_vertical(b, dt);

    if (b->rect.x < -20.0f || b->rect.x > SCREEN_WIDTH + 20.0f || b->rect.y > SCREEN_HEIGHT + 20.0f) {
        b->active = false;
    }
}

void barrel_draw(const Barrel *b, const Assets *a) {
    float scale = b->rect.width / (float)a->bomb.width;
    DrawTextureEx(a->bomb, (Vector2){ b->rect.x, b->rect.y }, 0.0f, scale, WHITE);
}