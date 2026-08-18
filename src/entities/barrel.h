#ifndef BARREL_H
#define BARREL_H

#include "raylib.h"
#include "level.h"

/* An enemy barrel: rolling hazard with gravity and collision state. */
typedef struct Barrel {
    Rectangle rect;    /* position and size on screen */
    float vx, vy;      /* current horizontal/vertical velocity */
    bool on_ground;    /* true while rolling on a platform */
    bool active;       /* false when despawned and free to reuse */
} Barrel;

/* Activate a barrel at pos, rolling in the given direction. */
void barrel_spawn(Barrel *b, Vector2 pos, float vx);

/* Advance barrel physics and collisions by dt. */
void barrel_update(Barrel *b, float dt);

/* Render the barrel as a wooden circle. */
void barrel_draw(const Barrel *b);

#endif