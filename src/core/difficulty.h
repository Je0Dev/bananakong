#ifndef DIFFICULTY_H
#define DIFFICULTY_H

#include "constants.h"

/* Tunables that scale with the current score. */
typedef struct Difficulty {
    float spawn_interval; /* seconds between barrel throws */
    float barrel_speed;   /* initial horizontal speed of a thrown barrel */
} Difficulty;

/* Difficulty ramps linearly from the base values to the capped values as
 * the score approaches DIFFICULTY_RAMP_SCORE. Pure and testable. */
Difficulty difficulty_for_score(int score);

/* Level baseline: harder each level, tightened by last-level performance
 * (faster clear and more stomps). Pure and testable. */
Difficulty difficulty_for_level(int level_index, float clear_time, int stomps);

/* Difficulty for live play: the level/performance baseline, then the score
 * ramp pushes toward the hard cap as points grow. Pure and testable. */
Difficulty difficulty_for_state(int level_index, float clear_time, int stomps, int score);

#endif