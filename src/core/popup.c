#include "popup.h"
#include "constants.h"

void popup_spawn(Popup *popups, int count, Vector2 pos, int points) {
    for (int i = 0; i < count; i++) {
        if (!popups[i].active) {
            popups[i].pos = pos;
            popups[i].points = points;
            popups[i].age = 0.0f;
            popups[i].active = true;
            return;
        }
    }
}

void popup_update(Popup *popups, int count, float dt) {
    for (int i = 0; i < count; i++) {
        if (!popups[i].active) continue;
        popups[i].age += dt;
        if (popups[i].age >= POPUP_LIFETIME) popups[i].active = false;
    }
}

void popup_draw(const Popup *popups, int count) {
    for (int i = 0; i < count; i++) {
        if (!popups[i].active) continue;
        float alpha = 1.0f - popups[i].age / POPUP_LIFETIME;
        float y = popups[i].pos.y - popups[i].age * POPUP_RISE;
        DrawText(TextFormat("+%d", popups[i].points), (int)popups[i].pos.x,
                 (int)y, 16, (Color){255, 255, 255, (unsigned char)(255 * alpha)});
    }
}