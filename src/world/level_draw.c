#include "level.h"
#include "assets.h"
#include "constants.h"

static void level_draw_background(const Assets *a) {
    Rectangle src = { 0, 0, (float)a->background.width, (float)a->background.height };
    Rectangle dst = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
    DrawTexturePro(a->background, src, dst, (Vector2){0, 0}, 0.0f, WHITE);
}

static void level_draw_tile(const Assets *a, char t, int col, int row) {
    float x = (float)(col * TILE_SIZE);
    float y = (float)(row * TILE_SIZE);
    float s = (float)TILE_SIZE / 64.0f; /* Kenney tiles are 64px */

    if (t == '#') {
        DrawTextureEx(a->tile_grass, (Vector2){x, y}, 0.0f, s, WHITE);
    } else if (t == 'L') {
        /* Pick top/middle/bottom by whether a ladder continues above/below. */
        Texture2D tex = a->ladder_middle;
        if (!level_is_ladder(col, row - 1)) tex = a->ladder_top;
        else if (!level_is_ladder(col, row + 1)) tex = a->ladder_bottom;
        DrawTextureEx(tex, (Vector2){x, y}, 0.0f, s, WHITE);
    } else if (t == 'G') {
        bool wave = ((int)(GetTime() * 4.0f)) % 2 == 0;
        DrawTextureEx(wave ? a->flag_a : a->flag_b, (Vector2){x, y}, 0.0f, s, WHITE);
    }
}

void level_draw(const Assets *a) {
    level_draw_background(a);
    for (int row = 0; row < GRID_ROWS; row++) {
        for (int col = 0; col < GRID_COLS; col++) {
            level_draw_tile(a, level_tile_at(col, row), col, row);
        }
    }
}