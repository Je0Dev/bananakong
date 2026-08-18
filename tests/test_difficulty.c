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

static void test_level_monotonic(void) {
    /* Slow clear + no stomps isolates the pure level ramp. */
    Difficulty a = difficulty_for_level(1, PERF_TIME_TARGET, 0);
    Difficulty b = difficulty_for_level(3, PERF_TIME_TARGET, 0);
    Difficulty c = difficulty_for_level(10, PERF_TIME_TARGET, 0);
    CHECK(b.spawn_interval < a.spawn_interval);
    CHECK(b.barrel_speed > a.barrel_speed);
    CHECK(c.spawn_interval >= DIFFICULTY_MIN_INTERVAL);
    CHECK(c.barrel_speed <= DIFFICULTY_MAX_SPEED);
}

static void test_performance_tightens(void) {
    Difficulty slow = difficulty_for_level(2, PERF_TIME_TARGET, 0);
    Difficulty fast = difficulty_for_level(2, 10.0f, PERF_STOMP_TARGET);
    CHECK(fast.spawn_interval <= slow.spawn_interval);
    CHECK(fast.barrel_speed >= slow.barrel_speed);
}

static void test_level_clamped(void) {
    Difficulty d = difficulty_for_level(50, PERF_TIME_TARGET, 0);
    CHECK(d.spawn_interval == DIFFICULTY_MIN_INTERVAL);
    CHECK(d.barrel_speed == DIFFICULTY_MAX_SPEED);
}

static void test_state_baseline_and_ramp(void) {
    Difficulty base = difficulty_for_level(2, PERF_TIME_TARGET, 0);
    Difficulty low = difficulty_for_state(2, PERF_TIME_TARGET, 0, 0);
    CHECK(low.spawn_interval == base.spawn_interval);
    CHECK(low.barrel_speed == base.barrel_speed);

    Difficulty high = difficulty_for_state(2, PERF_TIME_TARGET, 0, DIFFICULTY_RAMP_SCORE);
    CHECK(high.spawn_interval == DIFFICULTY_MIN_INTERVAL);
    CHECK(high.barrel_speed == DIFFICULTY_MAX_SPEED);
}

void test_difficulty(void) {
    test_base_values();
    test_capped_values();
    test_midpoint();
    test_clamped_at_extremes();
    test_monotonic();
    test_level_monotonic();
    test_performance_tightens();
    test_level_clamped();
    test_state_baseline_and_ramp();
}