#include "kong.h"
#include "constants.h"
#include <math.h>

static Color kong_tint = { 120, 80, 45, 255 };

void kong_draw(const Kong *k, const Assets *a) {
    /* The Kenney pack has no gorilla sprite, so Kong reuses the hero sprite
     * scaled up and tinted dark brown to read as the big boss. */
    float scale = (float)KONG_HEIGHT / (float)a->player_idle.height;
    float rot = 0.0f;

    /* Wind-up leans forward as a throw approaches; recoil snaps back after. */
    if (k->throw_timer < 0.3f) {
        rot = -10.0f * (1.0f - k->throw_timer / 0.3f);
    }
    if (k->recoil_timer > 0.0f) {
        rot = 8.0f * (k->recoil_timer / KONG_RECOIL_TIME);
    }

    /* Walk cycle while pacing; freeze on the idle frame while turning. */
    Texture2D tex = a->player_idle;
    if (k->turn_timer <= 0.0f) {
        bool frame = ((int)(k->anim_timer * 6.0f)) % 2 == 0;
        tex = frame ? a->player_walk_a : a->player_walk_b;
    }

    /* Mirror so Kong faces the way he walks. */
    Rectangle src = { 0, 0, (float)tex.width, (float)tex.height };
    if (k->patrol_dir < 0.0f) src.width = -src.width;
    float dw = (float)tex.width * scale;
    float dh = (float)tex.height * scale;
    Rectangle dst = { k->rect.x + KONG_WIDTH / 2.0f, k->rect.y + KONG_HEIGHT / 2.0f, dw, dh };
    DrawTexturePro(tex, src, dst, (Vector2){ dw / 2.0f, dh / 2.0f }, rot, kong_tint);

    /* A barrel bobs in the throwing hand, rising as a throw winds up. */
    float bob = sinf(k->anim_timer * 5.0f) * 4.0f;
    if (k->throw_timer < 0.3f) bob += 10.0f * (1.0f - k->throw_timer / 0.3f);
    float hand = k->throw_left ? -KONG_WIDTH / 2.0f : KONG_WIDTH / 2.0f;
    float bx = k->rect.x + KONG_WIDTH / 2.0f + hand;
    if (bx < 0.0f) bx = 0.0f;
    if (bx > KONG_PLATFORM_RIGHT - BARREL_SIZE) bx = KONG_PLATFORM_RIGHT - BARREL_SIZE;
    Vector2 bp = { bx, k->rect.y + 14.0f + bob };
    DrawTextureEx(a->bomb, bp, 0.0f, (float)BARREL_SIZE / (float)a->bomb.width, WHITE);
}