#include "player.h"

void player_draw(const Player *p) {
    if (p->invuln_timer > 0.0f && ((int)(p->invuln_timer * 12)) % 2 == 0) return;

    float x = p->rect.x;
    float y = p->rect.y;
    DrawEllipse(x + PLAYER_WIDTH / 2.0f, y + PLAYER_HEIGHT / 2.0f, PLAYER_WIDTH / 2.0f, PLAYER_HEIGHT / 2.0f, YELLOW);
    DrawEllipse(x + PLAYER_WIDTH / 2.0f, y + PLAYER_HEIGHT / 2.0f + 2.0f, PLAYER_WIDTH / 2.0f - 1.0f, PLAYER_HEIGHT / 2.0f - 2.0f, (Color){235, 195, 30, 255});
    DrawCircle(x + PLAYER_WIDTH - 1.0f, y + 2.0f, 2.0f, (Color){120, 80, 20, 255});
    DrawCircle(x + PLAYER_WIDTH / 2.0f + 2.0f, y + PLAYER_HEIGHT / 2.0f - 2.0f, 1.5f, BLACK);
}