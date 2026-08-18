#include "kong.h"
#include "constants.h"
#include <math.h>

void kong_draw(const Kong *k, const Assets *a) {
    /* The Kenney pack has no gorilla sprite, so Kong reuses the hero sprite
     * scaled up and tinted dark brown to read as the big boss. */
    float scale = (float)KONG_HEIGHT / (float)a->player_idle.height;
    Rectangle src = { 0, 0, (float)a->player_idle.width, (float)a->player_idle.height };
    Rectangle dst = { k->rect.x, k->rect.y,
                      (float)a->player_idle.width * scale, (float)a->player_idle.height * scale };
    DrawTexturePro(a->player_idle, src, dst, (Vector2){0, 0}, 0.0f, (Color){120, 80, 45, 255});

    /* A barrel bobs in front of him, winding up as a throw approaches. */
    float bob = sinf(k->anim_timer * 5.0f) * 4.0f;
    if (k->throw_timer < 0.3f) bob += 10.0f * (1.0f - k->throw_timer / 0.3f);
    float bs = (float)BARREL_SIZE / (float)a->bomb.width;
    Vector2 bp = { k->rect.x - 12.0f, k->rect.y + 14.0f + bob };
    if (bp.x < 0.0f) bp.x = 0.0f;
    DrawTextureEx(a->bomb, bp, 0.0f, bs, WHITE);
}