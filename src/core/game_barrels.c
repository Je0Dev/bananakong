#include "game.h"
#include "constants.h"
#include "scoring.h"

void game_spawn_barrel(Game *g) {
    for (int i = 0; i < MAX_BARRELS; i++) {
        if (!g->barrels[i].active) {
            /* Barrels leave Kong's front hand at platform level, rolling in
             * the direction toggled for this throw. */
            Vector2 pos = { g->kong.rect.x - BARREL_SIZE,
                            KONG_PLATFORM_ROW * TILE_SIZE - BARREL_SIZE };
            float speed = g->diff.barrel_speed * (g->kong.throw_left ? -1.0f : 1.0f);
            barrel_spawn(&g->barrels[i], pos, speed);
            g->kong.throw_left = !g->kong.throw_left;
            assets_play(&g->assets, SND_THROW);
            break;
        }
    }
}

void game_update_barrels(Game *g, float dt) {
    for (int i = 0; i < MAX_BARRELS; i++) {
        bool was_active = g->barrels[i].active;
        if (g->barrels[i].active) barrel_update(&g->barrels[i], dt);
        if (was_active && !g->barrels[i].active) {
            Vector2 pos = { g->barrels[i].rect.x, g->barrels[i].rect.y };
            scoring_barrel_popped(g, pos);
            assets_play(&g->assets, SND_COIN);
        }
    }
}