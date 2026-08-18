#include "game.h"
#include "constants.h"
#include "physics.h"
#include "highscore.h"
#include "scoring.h"

static void game_reset(Game *g) {
    level_init(&g->level);
    g->player.lives = START_LIVES;
    g->spawn_right = false; /* fresh run starts bottom-left */
    player_reset(&g->player, g->level.spawn_left);
    kong_init(&g->kong);
    for (int i = 0; i < MAX_BARRELS; i++) g->barrels[i].active = false;
    for (int i = 0; i < MAX_POPUPS; i++) g->popups[i].active = false;
    g->score = 0;
    g->diff = difficulty_for_score(0);
    g->state = GS_PLAYING;
}

void game_init(Game *g) {
    level_init(&g->level);
    g->state = GS_TITLE;
    g->screen_timer = 0.0f;
    g->best = highscore_load(HIGHSCORE_FILE);
}

static void game_check_goal(Game *g) {
    int center_col = physics_tile_col(g->player.rect.x + g->player.rect.width / 2.0f, TILE_SIZE);
    int feet_row = physics_tile_row(g->player.rect.y + g->player.rect.height, TILE_SIZE);
    int goal_col = physics_tile_col(g->level.goal.x, TILE_SIZE);
    int goal_row = physics_tile_row(g->level.goal.y, TILE_SIZE);
    if (center_col == goal_col && feet_row == goal_row) {
        scoring_player_win(g);
        assets_play(&g->assets, SND_GEM);
    }
}

static void game_update_playing(Game *g, float dt) {
    player_update(&g->player, dt, &g->assets);
    game_check_goal(g);
    if (g->state == GS_WIN) return;

    /* Difficulty follows the score, so the ramp updates as points are earned.
     * Kong throws a barrel each time his timer runs out, alternating sides. */
    g->diff = difficulty_for_score(g->score);
    kong_update(&g->kong, dt);
    if (g->kong.throw_timer <= 0.0f) {
        g->kong.throw_timer = g->diff.spawn_interval;
        game_spawn_barrel(g);
    }

    game_update_barrels(g, dt);
    popup_update(g->popups, MAX_POPUPS, dt);
    game_check_barrel_hit(g);
}

void game_update(Game *g, float dt) {
    g->screen_timer += dt;

    switch (g->state) {
        case GS_TITLE:
            if (IsKeyPressed(KEY_ENTER)) {
                assets_play(&g->assets, SND_SELECT);
                game_reset(g);
            }
            break;

        case GS_PLAYING:
            game_update_playing(g, dt);
            /* P pauses and resumes the action; overlay shown by game_draw. */
            if (IsKeyPressed(KEY_P)) g->state = GS_PAUSED;
            break;

        case GS_PAUSED:
            if (IsKeyPressed(KEY_P)) g->state = GS_PLAYING;
            break;

        /* Both end screens restart the run on Enter. */
        case GS_GAMEOVER:
        case GS_WIN:
            if (IsKeyPressed(KEY_ENTER)) {
                assets_play(&g->assets, SND_SELECT);
                game_reset(g);
            }
            break;
    }
}