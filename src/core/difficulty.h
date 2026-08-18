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

#endif