#ifndef CONSTANTS_H
#define CONSTANTS_H

#define SCREEN_WIDTH  640
#define SCREEN_HEIGHT 480

#define TILE_SIZE 16
#define GRID_COLS (SCREEN_WIDTH / TILE_SIZE)   /* 40 */
#define GRID_ROWS (SCREEN_HEIGHT / TILE_SIZE)  /* 30 */

#define PLAYER_SPEED  140.0f
#define JUMP_SPEED    260.0f
#define GRAVITY       520.0f
#define CLIMB_SPEED   110.0f

#define PLAYER_WIDTH  12
#define PLAYER_HEIGHT 14
#define BARREL_SIZE 14

#define MAX_BARRELS 6
#define BARREL_SPEED 70.0f
#define BARREL_SPAWN_INTERVAL 1.8f

/* Difficulty ramp: barrels get faster and more frequent as the score grows. */
#define DIFFICULTY_START_SPEED 70.0f
#define DIFFICULTY_MAX_SPEED 150.0f
#define DIFFICULTY_MAX_INTERVAL 1.8f
#define DIFFICULTY_MIN_INTERVAL 0.6f
#define DIFFICULTY_RAMP_SCORE 4000

/* Floating score popups: rise and fade out over their lifetime. */
#define MAX_POPUPS 8
#define POPUP_LIFETIME 0.9f
#define POPUP_RISE 30.0f

#define HIGHSCORE_FILE "bananakong_highscore.txt"

#define START_LIVES 3

#endif