#include "player.h"
#include "constants.h"
#include "physics.h"

void player_reset(Player *p, Vector2 spawn) {
    p->rect = (Rectangle){ spawn.x, spawn.y, PLAYER_WIDTH, PLAYER_HEIGHT };
    p->vx = 0.0f;
    p->vy = 0.0f;
    p->on_ground = false;
    p->climbing = false;
    p->facing_left = false;
    p->anim_timer = 0.0f;
    p->invuln_timer = 0.0f;
}

static void player_move_horizontal(Player *p, float dt, bool left, bool right) {
    float move = 0.0f;
    if (left) move -= 1.0f;
    if (right) move += 1.0f;
    if (move != 0.0f) p->facing_left = move < 0.0f;
    p->vx = move * PLAYER_SPEED;
    p->rect.x += p->vx * dt;
    if (level_solid_at_rect(p->rect)) {
        p->rect.x -= p->vx * dt;
        p->vx = 0.0f;
    }
}

static void player_resolve_vertical(Player *p, float dt) {
    p->vy = physics_gravity_step(p->vy, dt);
    p->rect.y += p->vy * dt;

    p->on_ground = false;
    if (!level_solid_at_rect(p->rect)) return;
    if (p->vy > 0.0f) {
        p->rect.y = physics_snap_bottom(p->rect.y + p->rect.height, TILE_SIZE) - p->rect.height;
        p->vy = 0.0f;
        p->on_ground = true;
    } else if (p->vy < 0.0f) {
        p->rect.y = physics_snap_top(p->rect.y, TILE_SIZE);
        p->vy = 0.0f;
    }
}

static void player_clamp_bounds(Player *p) {
    if (p->rect.x < 0) p->rect.x = 0;
    if (p->rect.x + p->rect.width > SCREEN_WIDTH) p->rect.x = SCREEN_WIDTH - p->rect.width;
    if (p->rect.y < 0) p->rect.y = 0;
}

void player_update(Player *p, float dt, const Assets *a) {
    if (p->invuln_timer > 0.0f) p->invuln_timer -= dt;
    p->anim_timer += dt;

    bool left = IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A);
    bool right = IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D);
    bool up = IsKeyDown(KEY_UP) || IsKeyDown(KEY_W);
    bool down = IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S);
    bool jump = IsKeyPressed(KEY_SPACE);

    /* Climbing replaces normal movement until the ladder is left. */
    if (p->climbing) {
        player_update_climbing(p, dt, up, down, jump, left, right);
        return;
    }

    player_try_climb(p, up, down);
    if (p->climbing) return;

    player_move_horizontal(p, dt, left, right);

    if (jump && p->on_ground) {
        p->vy = -JUMP_SPEED;
        p->on_ground = false;
        assets_play(a, SND_JUMP);
    }

    player_resolve_vertical(p, dt);
    player_clamp_bounds(p);
}