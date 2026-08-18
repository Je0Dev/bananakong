#ifndef KONG_H
#define KONG_H

#include "raylib.h"
#include "assets.h"

/* The barrel-throwing boss standing on the top platform. */
typedef struct Kong {
    Rectangle rect;     /* body box resting on the top platform */
    float throw_timer;  /* seconds until the next barrel throw */
    bool throw_left;    /* throw direction, alternated each throw */
    float anim_timer;   /* accumulated time, drives the barrel bob */
} Kong;

/* Place Kong on the top platform and arm the first throw. */
void kong_init(Kong *k);

/* Count down the throw timer and advance the bob animation by dt. */
void kong_update(Kong *k, float dt);

/* Render Kong (tinted sprite) holding a bobbing barrel. */
void kong_draw(const Kong *k, const Assets *a);

#endif