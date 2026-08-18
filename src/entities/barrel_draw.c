#include "barrel.h"
#include "constants.h"
#include <math.h>

void barrel_draw(const Barrel *b, const Assets *a) {
    /* A hop lifts the sprite in an arc over hop_time (visual only). */
    float dy = 0.0f;
    if (b->hop_time > 0.0f) {
        float p = b->hop_time / BARREL_HOP_TIME; /* counts down 1 -> 0 */
        dy = -sinf(PI * p) * BARREL_HOP_HEIGHT;
    }
    float scale = b->rect.width / (float)a->bomb.width;
    DrawTextureEx(a->bomb, (Vector2){ b->rect.x, b->rect.y + dy }, 0.0f, scale, WHITE);
}