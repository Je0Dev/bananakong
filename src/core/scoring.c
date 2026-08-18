#include "scoring.h"
#include "constants.h"
#include "highscore.h"

static void scoring_popup_at_player(Game *g, int points) {
    Vector2 pos = { g->player.rect.x + g->player.rect.width / 2.0f,
                    g->player.rect.y };
    popup_spawn(g->popups, MAX_POPUPS, pos, points);
}

static void scoring_bump(Game *g) {
    g->score_flash = HUD_FLASH_TIME;
}

void scoring_barrel_popped(Game *g, Vector2 pos) {
    g->score += 10;
    scoring_bump(g);
    popup_spawn(g->popups, MAX_POPUPS, pos, 10);
}

void scoring_barrel_stomped(Game *g, Vector2 pos) {
    g->score += STOMP_SCORE;
    scoring_bump(g);
    popup_spawn(g->popups, MAX_POPUPS, pos, STOMP_SCORE);
}

static void scoring_commit_best(Game *g) {
    int prev = g->best;
    g->best = highscore_update(HIGHSCORE_FILE, g->score);
    g->new_best = g->best > prev;
}

void scoring_player_win(Game *g) {
    g->score += 100;
    scoring_bump(g);
    scoring_popup_at_player(g, 100);
    scoring_commit_best(g);
    g->state = GS_WIN;
}

void scoring_game_over(Game *g) {
    scoring_commit_best(g);
    g->state = GS_GAMEOVER;
}