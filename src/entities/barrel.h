#ifndef BARREL_H
#define BARREL_H

#include "raylib.h"
#include "level.h"
#include "assets.h"

/* An enemy barrel: rolling hazard with gravity and collision state. */
typedef struct Barrel {
    Rectangle rect;    /* position and size on screen */
    float vx, vy;      /* current horizontal/vertical velocity */
    bool on_ground;    /* true while rolling on a platform */
    bool flying;       /* true while arcing through the air (no wall bounce) */
    bool active;       /* false when despawned and free to reuse */
} Barrel;

/* Activate a barrel at pos, rolling in the given direction. */
void barrel_spawn(Barrel *b, Vector2 pos, float vx);

/* Activate a barrel thrown in a high arc, flying until it lands. */
void barrel_spawn_arc(Barrel *b, Vector2 pos, float vx, float vy);

/* Advance barrel physics and collisions by dt. */
void barrel_update(Barrel *b, float dt);

/* Render the barrel as the bomb sprite. */
void barrel_draw(const Barrel *b, const Assets *a);

#endif