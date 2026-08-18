#include "game.h"

static void game_draw_title(const Game *g) {
    int title_size = 52;
    int title_w = MeasureText("BANANAKONG", title_size);
    DrawText("BANANAKONG", (SCREEN_WIDTH - title_w) / 2, SCREEN_HEIGHT / 2 - 60, title_size, YELLOW);
    DrawText("a banana's climb", (SCREEN_WIDTH - MeasureText("a banana's climb", 20)) / 2, SCREEN_HEIGHT / 2, 20, (Color){200, 200, 200, 255});
    DrawText(TextFormat("BEST: %d", g->best), (SCREEN_WIDTH - MeasureText(TextFormat("BEST: %d", g->best), 20)) / 2, SCREEN_HEIGHT / 2 + 26, 20, (Color){180, 180, 180, 255});
    if (((int)(g->screen_timer * 2)) % 2 == 0) {
        DrawText("Press ENTER to start", (SCREEN_WIDTH - MeasureText("Press ENTER to start", 24)) / 2, SCREEN_HEIGHT / 2 + 56, 24, WHITE);
    }
}

static void game_draw_hud(const Game *g) {
    for (int i = 0; i < g->player.lives; i++) {
        DrawEllipse(24 + i * 22, 20, 8, 10, YELLOW);
        DrawCircle(32 + i * 22, 12, 2, (Color){120, 80, 20, 255});
    }
    DrawText(TextFormat("SCORE: %d", g->score), SCREEN_WIDTH - 160, 12, 20, WHITE);
}

static void game_draw_overlay(const Game *g) {
    DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, (Color){0, 0, 0, 170});
    const char *msg = (g->state == GS_WIN) ? "YOU WIN!" : "GAME OVER";
    Color c = (g->state == GS_WIN) ? (Color){120, 220, 80, 255} : (Color){230, 90, 70, 255};
    DrawText(msg, (SCREEN_WIDTH - MeasureText(msg, 48)) / 2, SCREEN_HEIGHT / 2 - 50, 48, c);
    DrawText(TextFormat("Score: %d", g->score), (SCREEN_WIDTH - MeasureText(TextFormat("Score: %d", g->score), 24)) / 2, SCREEN_HEIGHT / 2 + 10, 24, WHITE);
    DrawText(TextFormat("Best: %d", g->best), (SCREEN_WIDTH - MeasureText(TextFormat("Best: %d", g->best), 20)) / 2, SCREEN_HEIGHT / 2 + 40, 20, (Color){200, 200, 200, 255});
    DrawText("Press ENTER to play again", (SCREEN_WIDTH - MeasureText("Press ENTER to play again", 20)) / 2, SCREEN_HEIGHT / 2 + 72, 20, (Color){200, 200, 200, 255});
}

static void game_draw_paused(void) {
    DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, (Color){0, 0, 0, 120});
    DrawText("PAUSED", (SCREEN_WIDTH - MeasureText("PAUSED", 40)) / 2, SCREEN_HEIGHT / 2 - 20, 40, WHITE);
    DrawText("Press P to resume", (SCREEN_WIDTH - MeasureText("Press P to resume", 20)) / 2, SCREEN_HEIGHT / 2 + 30, 20, (Color){200, 200, 200, 255});
}

void game_draw(const Game *g) {
    ClearBackground(BLACK);

    if (g->state == GS_TITLE) {
        game_draw_title(g);
        return;
    }

    level_draw(&g->assets);
    for (int i = 0; i < MAX_BARRELS; i++) {
        if (g->barrels[i].active) barrel_draw(&g->barrels[i], &g->assets);
    }
    player_draw(&g->player, &g->assets);
    popup_draw(g->popups, MAX_POPUPS);
    game_draw_hud(g);

    if (g->state == GS_PAUSED) game_draw_paused();
    if (g->state == GS_GAMEOVER || g->state == GS_WIN) game_draw_overlay(g);
}