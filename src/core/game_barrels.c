#include "game.h"
#include "constants.h"
#include "scoring.h"

/* Barrels start at the top platform's right edge and roll left. */
static const Vector2 BARREL_SPAWN_POS = { 24.0f * TILE_SIZE, 6.0f * TILE_SIZE - BARREL_SIZE };

void game_spawn_barrel(Game *g) {
    for (int i = 0; i < MAX_BARRELS; i++) {
        if (!g->barrels[i].active) {
            barrel_spawn(&g->barrels[i], BARREL_SPAWN_POS, -g->diff.barrel_speed);
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