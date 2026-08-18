#include "test_util.h"
#include "level.h"
#include "constants.h"

static void test_bounds_safe(void) {
    CHECK(level_tile_at(-1, 0) == '.');
    CHECK(level_tile_at(0, -1) == '.');
    CHECK(level_tile_at(GRID_COLS, 0) == '.');
    CHECK(level_tile_at(0, GRID_ROWS) == '.');
    CHECK(!level_is_solid(-1, 0));
    CHECK(!level_is_ladder(GRID_COLS, 0));
}

static void test_ground(void) {
    CHECK(level_is_solid(0, GRID_ROWS - 1));
    CHECK(level_is_solid(GRID_COLS - 1, GRID_ROWS - 1));
    CHECK(level_is_solid(20, GRID_ROWS - 1));
    CHECK(!level_is_solid(0, GRID_ROWS - 2));
}

static void test_goal(void) {
    CHECK(level_is_goal(3, 6));
    CHECK(!level_is_goal(0, 6));
    CHECK(level_is_solid(3, 6));
}

static void test_ladders(void) {
    CHECK(level_is_ladder(10, 7));
    CHECK(level_is_ladder(20, 12));
    CHECK(!level_is_ladder(10, 6));
    CHECK(!level_is_ladder(11, 7));
    CHECK(!level_is_solid(10, 7));
}

static void test_rect_collision(void) {
    Rectangle on_ground = { 5.0f * TILE_SIZE, (GRID_ROWS - 1) * TILE_SIZE - 2.0f, 12.0f, 14.0f };
    CHECK(level_solid_at_rect(on_ground));

    Rectangle in_air = { 30.0f * TILE_SIZE, 120.0f, 12.0f, 14.0f };
    CHECK(!level_solid_at_rect(in_air));

    Rectangle on_ladder = { 10.0f * TILE_SIZE + 2.0f, 7.0f * TILE_SIZE, 12.0f, 14.0f };
    CHECK(level_ladder_at_rect(on_ladder));

    Rectangle off_ladder = { 12.0f * TILE_SIZE, 7.0f * TILE_SIZE, 12.0f, 14.0f };
    CHECK(!level_ladder_at_rect(off_ladder));
}

void test_level(void) {
    test_bounds_safe();
    test_ground();
    test_goal();
    test_ladders();
    test_rect_collision();
}