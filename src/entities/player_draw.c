#include "player.h"

static void player_draw_sprite(const Player *p, Texture2D tex) {
    /* Kenney characters are 128x128; scale to roughly two tiles tall. */
    float scale = 32.0f / (float)tex.height;
    float draw_w = (float)tex.width * scale;
    float draw_h = (float)tex.height * scale;
    Rectangle src = { 0, 0, (float)tex.width, (float)tex.height };
    Rectangle dst = { p->rect.x + p->rect.width / 2.0f - draw_w / 2.0f,
                      p->rect.y + p->rect.height / 2.0f - draw_h / 2.0f,
                      draw_w, draw_h };
    if (p->facing_left) src.width = -src.width; /* mirror around center */
    DrawTexturePro(tex, src, dst, (Vector2){0, 0}, 0.0f, WHITE);
}

static Texture2D player_pick_frame(const Player *p, const Assets *a) {
    if (p->invuln_timer > 0.0f) return a->player_hit;
    if (p->climbing) {
        bool frame = ((int)(p->anim_timer * 8.0f)) % 2 == 0;
        return frame ? a->player_climb_a : a->player_climb_b;
    }
    if (!p->on_ground) return a->player_jump;
    bool moving = p->vx != 0.0f;
    if (moving) {
        bool frame = ((int)(p->anim_timer * 8.0f)) % 2 == 0;
        return frame ? a->player_walk_a : a->player_walk_b;
    }
    return a->player_idle;
}

void player_draw(const Player *p, const Assets *a) {
    if (p->invuln_timer > 0.0f && ((int)(p->invuln_timer * 12)) % 2 == 0) return;
    player_draw_sprite(p, player_pick_frame(p, a));
}