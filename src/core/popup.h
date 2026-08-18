#ifndef POPUP_H
#define POPUP_H

#include "raylib.h"

/* A floating "+points" label that rises and fades away. */
typedef struct Popup {
    Vector2 pos;  /* spawn position (top-center of the source) */
    int points;   /* value shown in the label */
    float age;    /* seconds since spawn */
    bool active;  /* false once faded and free to reuse */
} Popup;

/* Activate the first free popup slot with the given points at pos. */
void popup_spawn(Popup *popups, int count, Vector2 pos, int points);

/* Age every active popup, deactivating it past POPUP_LIFETIME. */
void popup_update(Popup *popups, int count, float dt);

/* Render all active popups rising and fading. */
void popup_draw(const Popup *popups, int count);

#endif