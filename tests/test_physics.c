#include "test_util.h"
#include "physics.h"
#include "constants.h"

static void test_gravity(void) {
    CHECK(physics_gravity_step(0.0f, 1.0f) == GRAVITY);
    CHECK(physics_gravity_step(10.0f, 2.0f) == 10.0f + GRAVITY * 2.0f);
}

static void test_snap(void) {
    CHECK(physics_snap_bottom(16.5f, 16.0f) == 16.0f);
    CHECK(physics_snap_bottom(31.9f, 16.0f) == 16.0f);
    CHECK(physics_snap_bottom(32.0f, 16.0f) == 32.0f);
    CHECK(physics_snap_top(15.5f, 16.0f) == 16.0f);
    CHECK(physics_snap_top(16.0f, 16.0f) == 32.0f);
}

static void test_tile_conversion(void) {
    CHECK(physics_tile_col(15.0f, 16.0f) == 0);
    CHECK(physics_tile_col(16.0f, 16.0f) == 1);
    CHECK(physics_tile_col(40.0f, 16.0f) == 2);
    CHECK(physics_tile_row(0.0f, 16.0f) == 0);
    CHECK(physics_tile_row(95.0f, 16.0f) == 5);
}

static void test_aabb(void) {
    Rectangle a = { 0.0f, 0.0f, 10.0f, 10.0f };
    Rectangle inside = { 5.0f, 5.0f, 3.0f, 3.0f };
    Rectangle overlapping = { 9.0f, 9.0f, 10.0f, 10.0f };
    Rectangle touching_edge = { 10.0f, 0.0f, 5.0f, 5.0f };
    Rectangle far = { 30.0f, 30.0f, 5.0f, 5.0f };

    CHECK(physics_aabb_overlap(a, inside));
    CHECK(physics_aabb_overlap(a, overlapping));
    CHECK(!physics_aabb_overlap(a, touching_edge));
    CHECK(!physics_aabb_overlap(a, far));
}

void test_physics(void) {
    test_gravity();
    test_snap();
    test_tile_conversion();
    test_aabb();
}