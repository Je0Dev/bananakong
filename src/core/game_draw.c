#include "game.h"
#include "constants.h"
#include "ui_draw.h"
#include "hud.h"

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

    /* Info panel: best score, run seed, and the blinking start prompt. */
    Rectangle info = { 170, 248, 300, 140 };
    ui_draw_panel(a->ui_panel_light, info);
    DrawText(TextFormat("BEST: %d", g->best),
        (SCREEN_WIDTH - MeasureText(TextFormat("BEST: %d", g->best), 20)) / 2, 272, 20, UI_TAN);
    DrawText(TextFormat("seed: %u", g->run_seed),
        (SCREEN_WIDTH - MeasureText(TextFormat("seed: %u", g->run_seed), 16)) / 2, 302, 16, UI_TAN);
    if (((int)(g->screen_timer * 2)) % 2 == 0) {
        DrawText("Press ENTER to start",
            (SCREEN_WIDTH - MeasureText("Press ENTER to start", 22)) / 2, 334, 22, UI_INK);
    }
    DrawText("ARROWS/WASD move · SPACE jump · P pause",
        (SCREEN_WIDTH - MeasureText("ARROWS/WASD move · SPACE jump · P pause", 16)) / 2, 408, 16, UI_TAN);
}

static void game_draw_overlay(const Game *g) {
    DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, (Color){0, 0, 0, 170});
    ui_draw_panel(g->assets.ui_panel_dark, (Rectangle){140, 130, 360, 220});
    const char *msg = (g->state == GS_WIN)
        ? TextFormat("LEVEL %d CLEAR!", g->level_index)
        : "GAME OVER";
    Color c = (g->state == GS_WIN) ? (Color){70, 140, 40, 255} : (Color){180, 40, 30, 255};
    DrawText(msg, (SCREEN_WIDTH - MeasureText(msg, 44)) / 2, 158, 44, c);
    if (g->new_best) {
        DrawText("NEW BEST!",
            (SCREEN_WIDTH - MeasureText("NEW BEST!", 20)) / 2, 216, 20, (Color){255, 190, 60, 255});
    }
    DrawText(TextFormat("Score: %d", g->score),
        (SCREEN_WIDTH - MeasureText(TextFormat("Score: %d", g->score), 24)) / 2, 248, 24, UI_INK);
    DrawText(TextFormat("Best: %d", g->best),
        (SCREEN_WIDTH - MeasureText(TextFormat("Best: %d", g->best), 20)) / 2, 280, 20, UI_TAN);
    const char *prompt = (g->state == GS_WIN)
        ? TextFormat("Press ENTER for LEVEL %d", g->level_index + 1)
        : "Press ENTER to play again";
    DrawText(prompt, (SCREEN_WIDTH - MeasureText(prompt, 20)) / 2, 312, 20, UI_TAN);
}

static void game_draw_paused(const Assets *a) {
    DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, (Color){0, 0, 0, 120});
    ui_draw_panel(a->ui_panel_dark, (Rectangle){180, 168, 280, 124});
    DrawText("PAUSED", (SCREEN_WIDTH - MeasureText("PAUSED", 40)) / 2, 192, 40, UI_INK);
    DrawText("Press P to resume", (SCREEN_WIDTH - MeasureText("Press P to resume", 20)) / 2, 250, 20, UI_TAN);
}

static void game_draw_intro(const Game *g) {
    ui_draw_panel(g->assets.ui_panel_light, (Rectangle){190, 150, 260, 90});
    DrawText(TextFormat("LEVEL %d", g->level_index),
        (SCREEN_WIDTH - MeasureText(TextFormat("LEVEL %d", g->level_index), 36)) / 2, 168, 36, UI_INK);
    DrawText("climb to the flag!",
        (SCREEN_WIDTH - MeasureText("climb to the flag!", 16)) / 2, 212, 16, UI_TAN);
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
    hud_draw(g);

    if (g->state == GS_PLAYING && g->level_intro_timer > 0.0f) game_draw_intro(g);
    if (g->state == GS_PAUSED) game_draw_paused(&g->assets);
    if (g->state == GS_GAMEOVER || g->state == GS_WIN) game_draw_overlay(g);
}