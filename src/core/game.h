#ifndef GAME_H
#define GAME_H

#include "raylib.h"
#include "level.h"
#include "player.h"
#include "barrel.h"
#include "kong.h"
#include "difficulty.h"
#include "popup.h"
#include "assets.h"

/* Top-level game screens the state machine can be in. */
typedef enum GameState {
    GS_TITLE,    /* title screen, waiting for Enter */
    GS_PLAYING,  /* active gameplay */
    GS_PAUSED,   /* gameplay halted, P resumes */
    GS_GAMEOVER, /* player ran out of lives */
    GS_WIN       /* player reached the golden flag */
} GameState;

/* Full game state: level, player, barrels, and HUD counters. */
typedef struct Game {
    GameState state;
    Assets assets;   /* sprites and sounds, loaded once at startup */
    Level level;
    Player player;
    Kong kong;
    Barrel barrels[MAX_BARRELS];
    Popup popups[MAX_POPUPS];
    Difficulty diff;    /* current difficulty, scaled from the score */
    bool spawn_right;   /* next respawn goes to the right spawn point */
    int score;
    int best;           /* all-time best score, loaded from disk */
    float screen_timer; /* time on the current screen, drives blinking */
} Game;

/* Initialize the game to the title screen. */
void game_init(Game *g);

/* Advance the current screen (input, physics, state transitions). */
void game_update(Game *g, float dt);

/* Spawn a barrel from the top platform and update all barrels, scoring pops. */
void game_spawn_barrel(Game *g);
void game_update_barrels(Game *g, float dt);

/* Render everything for the current screen. */
void game_draw(const Game *g);

#endif