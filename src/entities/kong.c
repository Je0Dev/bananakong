#include "kong.h"
#include "constants.h"

void kong_init(Kong *k) {
    k->rect = (Rectangle){ KONG_X_TILE * TILE_SIZE,
                           KONG_PLATFORM_ROW * TILE_SIZE - KONG_HEIGHT,
                           KONG_WIDTH, KONG_HEIGHT };
    /* First barrel arrives quickly so gameplay starts immediately. */
    k->throw_timer = 1.0f;
    k->throw_left = true;
    k->anim_timer = 0.0f;
}

void kong_update(Kong *k, float dt) {
    k->throw_timer -= dt;
    k->anim_timer += dt;
}