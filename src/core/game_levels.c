#include "game.h"
#include "constants.h"
#include "level_gen.h"

/* Move to the next level after a win: keep score/lives/best, bump the level
 * index, generate the next seed, and respawn the player. */
void game_start_level(Game *g) {
    g->level_index++;
    g->run_seed = level_gen_next_seed(g->run_seed);
    level_init(&g->level, g->level_index, g->run_seed);
    g->spawn_right = false;
    player_reset(&g->player, g->level.spawn_left);
    kong_init(&g->kong);
    for (int i = 0; i < MAX_BARRELS; i++) g->barrels[i].active = false;
    for (int i = 0; i < MAX_POPUPS; i++) g->popups[i].active = false;
    g->level_elapsed = 0.0f;
    g->level_stomps = 0;
    g->level_clear_time = 0.0f;
    g->level_intro_timer = LEVEL_INTRO_TIME;
    g->state = GS_PLAYING;
}