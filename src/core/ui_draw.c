#include "ui_draw.h"
#include "constants.h"

void ui_draw_panel(Texture2D tex, Rectangle dest) {
    const float b = UI_PANEL_BORDER;
    const float sw = (float)tex.width;
    const float sh = (float)tex.height;
    const Vector2 origin = { 0.0f, 0.0f };

    if (dest.width < 2.0f * b || dest.height < 2.0f * b) {
        DrawTexturePro(tex, (Rectangle){0, 0, sw, sh}, dest, origin, 0.0f, WHITE);
        return;
    }

    /* Corners keep their rounded shape at native size. */
    DrawTexturePro(tex, (Rectangle){0, 0, b, b}, (Rectangle){dest.x, dest.y, b, b}, origin, 0.0f, WHITE);
    DrawTexturePro(tex, (Rectangle){sw - b, 0, b, b}, (Rectangle){dest.x + dest.width - b, dest.y, b, b}, origin, 0.0f, WHITE);
    DrawTexturePro(tex, (Rectangle){0, sh - b, b, b}, (Rectangle){dest.x, dest.y + dest.height - b, b, b}, origin, 0.0f, WHITE);
    DrawTexturePro(tex, (Rectangle){sw - b, sh - b, b, b}, (Rectangle){dest.x + dest.width - b, dest.y + dest.height - b, b, b}, origin, 0.0f, WHITE);

    /* Edge strips stretch along their axis only. */
    DrawTexturePro(tex, (Rectangle){b, 0, sw - 2.0f * b, b}, (Rectangle){dest.x + b, dest.y, dest.width - 2.0f * b, b}, origin, 0.0f, WHITE);
    DrawTexturePro(tex, (Rectangle){b, sh - b, sw - 2.0f * b, b}, (Rectangle){dest.x + b, dest.y + dest.height - b, dest.width - 2.0f * b, b}, origin, 0.0f, WHITE);
    DrawTexturePro(tex, (Rectangle){0, b, b, sh - 2.0f * b}, (Rectangle){dest.x, dest.y + b, b, dest.height - 2.0f * b}, origin, 0.0f, WHITE);
    DrawTexturePro(tex, (Rectangle){sw - b, b, b, sh - 2.0f * b}, (Rectangle){dest.x + dest.width - b, dest.y + b, b, dest.height - 2.0f * b}, origin, 0.0f, WHITE);

    /* The interior fill stretches both ways. */
    DrawTexturePro(tex, (Rectangle){b, b, sw - 2.0f * b, sh - 2.0f * b}, (Rectangle){dest.x + b, dest.y + b, dest.width - 2.0f * b, dest.height - 2.0f * b}, origin, 0.0f, WHITE);
}