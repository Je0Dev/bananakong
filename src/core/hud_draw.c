#include "hud.h"
#include "constants.h"

void hud_draw(const Game *g) {
    /* Lives as little bananas at the top-left. */
    for (int i = 0; i < g->player.lives; i++) {
        DrawEllipse(24 + i * 22, 20, 8, 10, YELLOW);
        DrawCircle(32 + i * 22, 12, 2, (Color){120, 80, 20, 255});
    }

    /* Level badge and the live score, which flashes yellow when it changes. */
    DrawText(TextFormat("LVL %d", g->level_index), SCREEN_WIDTH - 160, 8, 16, (Color){200, 200, 200, 255});
    Color score_color = (g->score_flash > 0.0f) ? YELLOW : WHITE;
    DrawText(TextFormat("SCORE: %d", g->score), SCREEN_WIDTH - 160, 28, 20, score_color);

    /* Climb meter: filled by how far the player has risen toward the goal. */
    float top = (float)KONG_PLATFORM_ROW * TILE_SIZE;
    float bot = (float)(GRID_ROWS - 2) * TILE_SIZE;
    float feet = g->player.rect.y + g->player.rect.height;
    float p = (bot - feet) / (bot - top);
    if (p < 0.0f) p = 0.0f;
    if (p > 1.0f) p = 1.0f;
    Rectangle track = { (SCREEN_WIDTH - 160) / 2.0f, 12, 160, 10 };
    DrawRectangleRec(track, (Color){0, 0, 0, 120});
    DrawRectangleLinesEx(track, 1.0f, (Color){200, 200, 200, 255});
    DrawRectangle((int)track.x + 1, (int)track.y + 1,
                  (int)((track.width - 2) * p), (int)track.height - 2, YELLOW);
}