#include "difficulty.h"

static float clamp01(float t) {
    if (t > 1.0f) return 1.0f;
    if (t < 0.0f) return 0.0f;
    return t;
}

static float score_factor(int score) {
    return clamp01((float)score / DIFFICULTY_RAMP_SCORE);
}

Difficulty difficulty_for_score(int score) {
    float t = score_factor(score);
    Difficulty d;
    d.spawn_interval = DIFFICULTY_MAX_INTERVAL
                       + (DIFFICULTY_MIN_INTERVAL - DIFFICULTY_MAX_INTERVAL) * t;
    d.barrel_speed = DIFFICULTY_START_SPEED
                     + (DIFFICULTY_MAX_SPEED - DIFFICULTY_START_SPEED) * t;
    return d;
}

Difficulty difficulty_for_level(int level_index, float clear_time, int stomps) {
    int lv = (level_index < 1) ? 1 : level_index;
    Difficulty d;
    d.spawn_interval = DIFFICULTY_MAX_INTERVAL - (lv - 1) * DIFFICULTY_LEVEL_INTERVAL_STEP;
    d.barrel_speed = DIFFICULTY_START_SPEED + (lv - 1) * DIFFICULTY_LEVEL_SPEED_STEP;

    /* 0..1 blend of how well the player did on the last level. */
    float perf = 0.7f * clamp01((PERF_TIME_TARGET - clear_time) / PERF_TIME_TARGET)
               + 0.3f * clamp01((float)stomps / PERF_STOMP_TARGET);
    perf = clamp01(perf);
    d.spawn_interval -= perf * DIFFICULTY_PERF_INTERVAL_ADJ;
    d.barrel_speed += perf * DIFFICULTY_PERF_SPEED_ADJ;

    if (d.spawn_interval < DIFFICULTY_MIN_INTERVAL) d.spawn_interval = DIFFICULTY_MIN_INTERVAL;
    if (d.barrel_speed > DIFFICULTY_MAX_SPEED) d.barrel_speed = DIFFICULTY_MAX_SPEED;
    return d;
}

Difficulty difficulty_for_state(int level_index, float clear_time, int stomps, int score) {
    Difficulty base = difficulty_for_level(level_index, clear_time, stomps);
    float t = score_factor(score);
    Difficulty d;
    d.spawn_interval = base.spawn_interval
                       + (DIFFICULTY_MIN_INTERVAL - base.spawn_interval) * t;
    d.barrel_speed = base.barrel_speed
                     + (DIFFICULTY_MAX_SPEED - base.barrel_speed) * t;
    return d;
}