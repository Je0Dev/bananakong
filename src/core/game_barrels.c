#include "game.h"
#include "constants.h"
#include "physics.h"
#include "scoring.h"

/* Toggle the spawn side so consecutive respawns land on opposite ends. */
static Vector2 game_next_spawn(Game *g) {
    g->spawn_right = !g->spawn_right;
    return g->spawn_right ? g->level.spawn_right : g->level.spawn_left;
}

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

void game_check_barrel_hit(Game *g) {
    if (g->player.invuln_timer > 0.0f) return;
    for (int i = 0; i < MAX_BARRELS; i++) {
        if (!g->barrels[i].active) continue;
        if (!physics_aabb_overlap(g->player.rect, g->barrels[i].rect)) continue;

        /* Landing from above smashes the barrel: score, bounce, no life lost. */
        if (physics_stomp_hit(g->player.rect, g->barrels[i].rect, g->player.vy)) {
            Vector2 pos = { g->barrels[i].rect.x, g->barrels[i].rect.y };
            g->barrels[i].active = false;
            g->player.vy = -STOMP_BOUNCE_SPEED;
            g->player.on_ground = false;
            scoring_barrel_stomped(g, pos);
            assets_play(&g->assets, SND_COIN);
            continue;
        }

        g->player.lives--;
        g->player.invuln_timer = 1.0f;
        assets_play(&g->assets, SND_HURT);
        if (g->player.lives <= 0) {
            scoring_game_over(g);
        } else {
            player_reset(&g->player, game_next_spawn(g));
        }
        break;
    }
}