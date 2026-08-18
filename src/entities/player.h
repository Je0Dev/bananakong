#ifndef PLAYER_H
#define PLAYER_H

#include "raylib.h"
#include "level.h"

/* The banana hero: collision box plus movement/climbing state. */
typedef struct Player {
    Rectangle rect;    /* position and size on screen */
    float vx, vy;      /* current horizontal/vertical velocity */
    bool on_ground;    /* true while standing on a platform */
    bool climbing;     /* true while gripping a ladder */
    int lives;         /* remaining lives, 0 means game over */
    float invuln_timer;/* brief invulnerability after being hit */
} Player;

/* Reset the player to its spawn point and clear movement state. */
void player_reset(Player *p, Vector2 spawn);

/* Advance player physics, input handling, and collisions by dt. */
void player_update(Player *p, float dt);

/* Render the banana (blinks while invulnerable). */
void player_draw(const Player *p);

#endif