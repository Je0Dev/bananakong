#include "player.h"
#include "constants.h"
#include "physics.h"

void player_reset(Player *p, Vector2 spawn) {
    p->rect = (Rectangle){ spawn.x, spawn.y, PLAYER_WIDTH, PLAYER_HEIGHT };
    p->vx = 0.0f;
    p->vy = 0.0f;
    p->on_ground = false;
    p->climbing = false;
    p->invuln_timer = 0.0f;
}

static int player_center_col(const Player *p) {
    return physics_tile_col(p->rect.x + p->rect.width / 2.0f, TILE_SIZE);
}

static void player_snap_to_ladder(Player *p) {
    float cx = player_center_col(p) * TILE_SIZE + TILE_SIZE / 2.0f;
    p->rect.x = cx - p->rect.width / 2.0f;
}

static void player_update_climbing(Player *p, float dt, bool up, bool down, bool jump, bool left, bool right) {
    /* Stay locked to the ladder column while climbing. */
    player_snap_to_ladder(p);
    if (up) p->rect.y -= CLIMB_SPEED * dt;
    if (down) p->rect.y += CLIMB_SPEED * dt;

    /* Stepping onto a platform above/below ends the climb. */
    Rectangle feet = { p->rect.x, p->rect.y + p->rect.height - 2.0f, p->rect.width, 2.0f };
    if (level_solid_at_rect(feet)) {
        p->climbing = false;
        p->on_ground = true;
        p->rect.y = physics_snap_bottom(p->rect.y + p->rect.height, TILE_SIZE) - p->rect.height;
    }
    if (!level_ladder_at_rect(p->rect)) p->climbing = false;
    if (jump) {
        p->climbing = false;
        p->vy = -JUMP_SPEED;
    }
    if (left || right) p->climbing = false;
    if (p->rect.y < 0) p->rect.y = 0;
}

static void player_try_climb(Player *p, bool up, bool down) {
    if (p->climbing || (!up && !down)) return;
    int center_col = player_center_col(p);
    int feet_row = physics_tile_row(p->rect.y + p->rect.height + 1.0f, TILE_SIZE);
    /* Up grips the ladder the body already overlaps; down steps into the
     * ladder tile sitting right below the player's feet. */
    bool near_ladder = up && level_ladder_at_rect(p->rect);
    bool below_ladder = down && level_is_ladder(center_col, feet_row);
    if (near_ladder || below_ladder) {
        p->climbing = true;
        p->vx = 0.0f;
        p->vy = 0.0f;
        p->on_ground = false;
    }
}

static void player_move_horizontal(Player *p, float dt, bool left, bool right) {
    float move = 0.0f;
    if (left) move -= 1.0f;
    if (right) move += 1.0f;
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

void player_update(Player *p, float dt) {
    if (p->invuln_timer > 0.0f) p->invuln_timer -= dt;

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
    }

    player_resolve_vertical(p, dt);
    player_clamp_bounds(p);
}