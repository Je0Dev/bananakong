#include "difficulty.h"

Difficulty difficulty_for_score(int score) {
    float t = (float)score / DIFFICULTY_RAMP_SCORE;
    if (t > 1.0f) t = 1.0f;
    if (t < 0.0f) t = 0.0f;

    Difficulty d;
    d.spawn_interval = DIFFICULTY_MAX_INTERVAL
                       + (DIFFICULTY_MIN_INTERVAL - DIFFICULTY_MAX_INTERVAL) * t;
    d.barrel_speed = DIFFICULTY_START_SPEED
                     + (DIFFICULTY_MAX_SPEED - DIFFICULTY_START_SPEED) * t;
    return d;
}