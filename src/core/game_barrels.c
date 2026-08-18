#include "game.h"
#include "constants.h"
#include "scoring.h"

void game_spawn_barrel(Game *g) {
    for (int i = 0; i < MAX_BARRELS; i++) {
        if (!g->barrels[i].active) {
            /* Barrels leave Kong's front hand at platform level, kept inside
             * the platform bounds so they always land on solid ground. */
            float px = g->kong.throw_left ? g->kong.rect.x - BARREL_SIZE
                                          : g->kong.rect.x + KONG_WIDTH;
            if (px < 0.0f) px = 0.0f;
            if (px > KONG_PLATFORM_RIGHT - BARREL_SIZE) px = KONG_PLATFORM_RIGHT - BARREL_SIZE;
            Vector2 pos = { px, KONG_PLATFORM_ROW * TILE_SIZE - BARREL_SIZE };
            float dir = g->kong.throw_left ? -1.0f : 1.0f;
            /* One throw in three leaves Kong's hand as a high arc; it sails
             * over a floor or two and rolls once it touches down. */
            if (GetRandomValue(0, BARREL_ARC_ODDS - 1) == 0) {
                barrel_spawn_arc(&g->barrels[i], pos, dir * g->diff.barrel_speed * BARREL_ARC_SPEED_MUL,
                                 -BARREL_ARC_VY);
            } else {
                barrel_spawn(&g->barrels[i], pos, dir * g->diff.barrel_speed);
            }
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