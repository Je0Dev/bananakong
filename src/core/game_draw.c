#include "game.h"
#include "constants.h"
#include "ui_draw.h"

static void game_draw_title(const Game *g) {
    const Assets *a = &g->assets;

    /* Faded game backdrop behind the menus. */
    Rectangle bg_src = { 0, 0, (float)a->background.width, (float)a->background.height };
    Rectangle bg_dst = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
    DrawTexturePro(a->background, bg_src, bg_dst, (Vector2){0, 0}, 0.0f, (Color){255, 255, 255, 150});

    /* Banner: mascot sprite on the left, logo and tagline on the right. */
    Rectangle banner = { 50, 50, 540, 150 };
    ui_draw_panel(a->ui_panel_dark, banner);
    DrawTextureEx(a->player_idle, (Vector2){72, 93}, 0.5f, 0.0f, WHITE);
    int title_size = 40;
    int title_w = MeasureText("BANANAKONG", title_size);
    DrawText("BANANAKONG", 363 - title_w / 2, 78, title_size, UI_INK);
    DrawText("a banana's climb", (SCREEN_WIDTH - MeasureText("a banana's climb", 20)) / 2, 150, 20, UI_TAN);

    /* Info panel with the best score and the start prompt. */
    Rectangle info = { 170, 260, 300, 120 };
    ui_draw_panel(a->ui_panel_light, info);
    DrawText(TextFormat("BEST: %d", g->best),
        (SCREEN_WIDTH - MeasureText(TextFormat("BEST: %d", g->best), 20)) / 2, 290, 20, UI_TAN);
    if (((int)(g->screen_timer * 2)) % 2 == 0) {
        DrawText("Press ENTER to start",
            (SCREEN_WIDTH - MeasureText("Press ENTER to start", 22)) / 2, 326, 22, UI_INK);
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
    ui_draw_panel(g->assets.ui_panel_dark, (Rectangle){140, 140, 360, 200});
    const char *msg = (g->state == GS_WIN) ? "YOU WIN!" : "GAME OVER";
    Color c = (g->state == GS_WIN) ? (Color){70, 140, 40, 255} : (Color){180, 40, 30, 255};
    DrawText(msg, (SCREEN_WIDTH - MeasureText(msg, 44)) / 2, 178, 44, c);
    DrawText(TextFormat("Score: %d", g->score),
        (SCREEN_WIDTH - MeasureText(TextFormat("Score: %d", g->score), 24)) / 2, 238, 24, UI_INK);
    DrawText(TextFormat("Best: %d", g->best),
        (SCREEN_WIDTH - MeasureText(TextFormat("Best: %d", g->best), 20)) / 2, 270, 20, UI_TAN);
    DrawText("Press ENTER to play again",
        (SCREEN_WIDTH - MeasureText("Press ENTER to play again", 20)) / 2, 302, 20, UI_TAN);
}

static void game_draw_paused(const Assets *a) {
    DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, (Color){0, 0, 0, 120});
    ui_draw_panel(a->ui_panel_dark, (Rectangle){180, 168, 280, 124});
    DrawText("PAUSED", (SCREEN_WIDTH - MeasureText("PAUSED", 40)) / 2, 192, 40, UI_INK);
    DrawText("Press P to resume", (SCREEN_WIDTH - MeasureText("Press P to resume", 20)) / 2, 250, 20, UI_TAN);
}

void game_draw(const Game *g) {
    ClearBackground(BLACK);

    if (g->state == GS_TITLE) {
        game_draw_title(g);
        return;
    }

    level_draw(&g->assets);
    kong_draw(&g->kong, &g->assets);
    for (int i = 0; i < MAX_BARRELS; i++) {
        if (g->barrels[i].active) barrel_draw(&g->barrels[i], &g->assets);
    }
    player_draw(&g->player, &g->assets);
    popup_draw(g->popups, MAX_POPUPS);
    game_draw_hud(g);

    if (g->state == GS_PAUSED) game_draw_paused(&g->assets);
    if (g->state == GS_GAMEOVER || g->state == GS_WIN) game_draw_overlay(g);
}