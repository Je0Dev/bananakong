#include "player.h"
#include "constants.h"
#include "physics.h"

static int player_center_col(const Player *p) {
    return physics_tile_col(p->rect.x + p->rect.width / 2.0f, TILE_SIZE);
}

static void player_snap_to_ladder(Player *p) {
    float cx = player_center_col(p) * TILE_SIZE + TILE_SIZE / 2.0f;
    p->rect.x = cx - p->rect.width / 2.0f;
}

void player_update_climbing(Player *p, float dt, bool up, bool down, bool jump, bool left, bool right) {
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

void player_try_climb(Player *p, bool up, bool down) {
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