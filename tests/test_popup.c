#include "test_util.h"
#include "popup.h"
#include "constants.h"

static void test_spawn_fills_first_slot(void) {
    Popup popups[MAX_POPUPS] = {0};
    Vector2 pos = { 100.0f, 50.0f };
    popup_spawn(popups, MAX_POPUPS, pos, 10);
    CHECK(popups[0].active);
    CHECK(popups[0].points == 10);
    CHECK(popups[0].pos.x == pos.x);
    CHECK(popups[0].age == 0.0f);
    CHECK(!popups[1].active);
}

static void test_update_ages_and_expires(void) {
    Popup popups[MAX_POPUPS] = {0};
    popup_spawn(popups, MAX_POPUPS, (Vector2){0, 0}, 50);
    popup_update(popups, MAX_POPUPS, 0.5f);
    CHECK(popups[0].active);
    CHECK(popups[0].age > 0.4f);
    popup_update(popups, MAX_POPUPS, POPUP_LIFETIME);
    CHECK(!popups[0].active);
}

static void test_full_pool_ignores_new(void) {
    Popup popups[MAX_POPUPS] = {0};
    for (int i = 0; i < MAX_POPUPS; i++) {
        popup_spawn(popups, MAX_POPUPS, (Vector2){0, 0}, 10);
    }
    popup_spawn(popups, MAX_POPUPS, (Vector2){0, 0}, 10);
    int active = 0;
    for (int i = 0; i < MAX_POPUPS; i++) if (popups[i].active) active++;
    CHECK(active == MAX_POPUPS);
}

static void test_slot_reuse_after_expiry(void) {
    Popup popups[MAX_POPUPS] = {0};
    popup_spawn(popups, MAX_POPUPS, (Vector2){0, 0}, 10);
    popup_update(popups, MAX_POPUPS, POPUP_LIFETIME);
    popup_spawn(popups, MAX_POPUPS, (Vector2){1, 2}, 100);
    CHECK(popups[0].active);
    CHECK(popups[0].points == 100);
}

void test_popup(void) {
    test_spawn_fills_first_slot();
    test_update_ages_and_expires();
    test_full_pool_ignores_new();
    test_slot_reuse_after_expiry();
}