#ifndef PLAYER_H
#define PLAYER_H

#include "raylib.h"
#include "level.h"
#include "assets.h"

/* The banana hero: collision box plus movement/climbing state. */
typedef struct Player {
    Rectangle rect;    /* position and size on screen */
    float vx, vy;      /* current horizontal/vertical velocity */
    bool on_ground;    /* true while standing on a platform */
    bool climbing;     /* true while gripping a ladder */
    bool facing_left;  /* last horizontal facing, drives sprite flip */
    float anim_timer;  /* accumulated time, drives walk/climb frames */
    int lives;         /* remaining lives, 0 means game over */
    float invuln_timer;/* brief invulnerability after being hit */
} Player;

/* Reset the player to its spawn point and clear movement state. */
void player_reset(Player *p, Vector2 spawn);

/* Ladder logic: grip/release handling, split out to keep player.c small. */
void player_update_climbing(Player *p, float dt, bool up, bool down, bool jump, bool left, bool right);
void player_try_climb(Player *p, bool up, bool down);

/* Advance player physics, input handling, and collisions by dt. */
void player_update(Player *p, float dt, const Assets *a);

/* Render the banana (blinks while invulnerable). */
void player_draw(const Player *p, const Assets *a);

#endif