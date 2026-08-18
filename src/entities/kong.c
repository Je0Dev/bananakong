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
    k->patrol_dir = -1.0f;
    k->turn_timer = 0.0f;
    k->recoil_timer = 0.0f;
}

void kong_update(Kong *k, float dt) {
    k->throw_timer -= dt;
    k->anim_timer += dt;
    if (k->turn_timer > 0.0f) {
        k->turn_timer -= dt; /* pause to "turn around" at the platform edge */
        return;
    }
    if (k->recoil_timer > 0.0f) k->recoil_timer -= dt;

    /* Pace the platform, turning around at both ends. */
    k->rect.x += KONG_SPEED * k->patrol_dir * dt;
    if (k->rect.x <= 0.0f) {
        k->rect.x = 0.0f;
        k->patrol_dir = 1.0f;
        k->turn_timer = KONG_TURN_TIME;
    }
    if (k->rect.x >= KONG_PLATFORM_RIGHT - KONG_WIDTH) {
        k->rect.x = KONG_PLATFORM_RIGHT - KONG_WIDTH;
        k->patrol_dir = -1.0f;
        k->turn_timer = KONG_TURN_TIME;
    }
}