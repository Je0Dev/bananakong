#include "barrel.h"
#include "constants.h"
#include <math.h>

void barrel_draw(const Barrel *b, const Assets *a) {
    float w = b->rect.width;
    float h = b->rect.height;
    float dy = 0.0f;
    float rot = 0.0f;

    if (b->flying) {
        /* Tumble along the arc's velocity direction. */
        rot = atan2f(b->vy, b->vx) * RAD2DEG;
    } else if (b->vx != 0.0f) {
        /* Rolling: reuse the accumulated spin from barrel_move_horizontal. */
        rot = b->roll_angle;
    }

    /* A hop lifts the sprite in a sine arc; it squashes at the apex. */
    if (b->hop_time > 0.0f) {
        float p = b->hop_time / BARREL_HOP_TIME; /* counts down 1 -> 0 */
        float squish = sinf(PI * p);
        dy = -squish * BARREL_HOP_HEIGHT;
        w *= 1.0f + 0.25f * squish;
        h *= 1.0f - 0.25f * squish;
    }

    Rectangle src = { 0, 0, (float)a->bomb.width, (float)a->bomb.height };
    Rectangle dst = { b->rect.x + b->rect.width / 2.0f,
                      b->rect.y + dy + b->rect.height / 2.0f, w, h };
    DrawTexturePro(a->bomb, src, dst, (Vector2){ w / 2.0f, h / 2.0f }, rot, WHITE);
}