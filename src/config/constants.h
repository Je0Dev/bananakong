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

/* One in N throws is a high arc that clears a floor or two before rolling. */
#define BARREL_ARC_ODDS 3
#define BARREL_ARC_VY 230.0f
#define BARREL_ARC_SPEED_MUL 1.6f

/* The barrel-throwing boss stands at the right end of the top platform. */
#define KONG_PLATFORM_ROW 6
#define KONG_PLATFORM_RIGHT (26 * TILE_SIZE) /* platform spans cols 0-25 */
#define KONG_X_TILE 18
#define KONG_WIDTH 80
#define KONG_HEIGHT 80
#define KONG_SPEED 40.0f
#define KONG_TURN_TIME 0.35f   /* pause when reversing at a platform edge */
#define KONG_RECOIL_TIME 0.18f /* throw snap-back after releasing a barrel */

/* Difficulty ramp: barrels get faster and more frequent as the score grows. */
#define DIFFICULTY_START_SPEED 70.0f
#define DIFFICULTY_MAX_SPEED 150.0f
#define DIFFICULTY_MAX_INTERVAL 1.8f
#define DIFFICULTY_MIN_INTERVAL 0.6f
#define DIFFICULTY_RAMP_SCORE 4000

/* Per-level + performance difficulty: each level tightens the baseline, and
 * clearing fast with plenty of stomps makes the next level slightly harder. */
#define DIFFICULTY_LEVEL_INTERVAL_STEP 0.2f
#define DIFFICULTY_LEVEL_SPEED_STEP 12.0f
#define DIFFICULTY_PERF_INTERVAL_ADJ 0.25f
#define DIFFICULTY_PERF_SPEED_ADJ 20.0f
#define PERF_TIME_TARGET 45.0f
#define PERF_STOMP_TARGET 6

/* Floating score popups: rise and fade out over their lifetime. */
#define MAX_POPUPS 8
#define POPUP_LIFETIME 0.9f
#define POPUP_RISE 30.0f

/* Stomping a barrel from above breaks it, bounces the player, and scores. */
#define STOMP_SCORE 50
#define STOMP_BOUNCE_SPEED 160.0f

/* Barrels visibly hop when reversing or dropping onto a lower step. */
#define BARREL_HOP_TIME 0.25f
#define BARREL_HOP_HEIGHT 10.0f

/* Kenney UI panels (32px tiles) have a 4px border on every side; a 9-slice
 * keeps corners and the border at native scale while the middle stretches. */
#define UI_PANEL_BORDER 4

/* Ink colors that read well on the cream Kenney panel interior. */
#define UI_INK   (Color){109, 75, 39, 255}
#define UI_TAN   (Color){163, 112, 58, 255}
#define UI_CREAM (Color){255, 241, 210, 255}

#define HIGHSCORE_FILE "bananakong_highscore.txt"

#define START_LIVES 3

/* HUD + screen-transition timing. */
#define LEVEL_INTRO_TIME 1.2f
#define HUD_FLASH_TIME 0.3f

#endif