#include "test_util.h"
#include "difficulty.h"

static void test_base_values(void) {
    Difficulty d = difficulty_for_score(0);
    CHECK(d.spawn_interval == DIFFICULTY_MAX_INTERVAL);
    CHECK(d.barrel_speed == DIFFICULTY_START_SPEED);
}

static void test_capped_values(void) {
    Difficulty d = difficulty_for_score(DIFFICULTY_RAMP_SCORE);
    CHECK(d.spawn_interval == DIFFICULTY_MIN_INTERVAL);
    CHECK(d.barrel_speed == DIFFICULTY_MAX_SPEED);
}

static void test_midpoint(void) {
    /* Half the ramp score lands exactly between the min and max values. */
    Difficulty d = difficulty_for_score(DIFFICULTY_RAMP_SCORE / 2);
    CHECK(d.spawn_interval == (DIFFICULTY_MAX_INTERVAL + DIFFICULTY_MIN_INTERVAL) / 2.0f);
    CHECK(d.barrel_speed == (DIFFICULTY_START_SPEED + DIFFICULTY_MAX_SPEED) / 2.0f);
}

static void test_clamped_at_extremes(void) {
    Difficulty high = difficulty_for_score(DIFFICULTY_RAMP_SCORE * 2);
    CHECK(high.spawn_interval == DIFFICULTY_MIN_INTERVAL);
    CHECK(high.barrel_speed == DIFFICULTY_MAX_SPEED);
}

static void test_monotonic(void) {
    Difficulty low = difficulty_for_score(0);
    Difficulty high = difficulty_for_score(DIFFICULTY_RAMP_SCORE);
    CHECK(high.spawn_interval < low.spawn_interval);
    CHECK(high.barrel_speed > low.barrel_speed);
}

void test_difficulty(void) {
    test_base_values();
    test_capped_values();
    test_midpoint();
    test_clamped_at_extremes();
    test_monotonic();
}