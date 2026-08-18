#ifndef GAME_H
#define GAME_H

#include "raylib.h"
#include "level.h"
#include "player.h"
#include "barrel.h"

/* Top-level game screens the state machine can be in. */
typedef enum GameState {
    GS_TITLE,    /* title screen, waiting for Enter */
    GS_PLAYING,  /* active gameplay */
    GS_GAMEOVER, /* player ran out of lives */
    GS_WIN       /* player reached the golden flag */
} GameState;

/* Full game state: level, player, barrels, and HUD counters. */
typedef struct Game {
    GameState state;
    Level level;
    Player player;
    Barrel barrels[MAX_BARRELS];
    float spawn_timer;  /* countdown until the next barrel spawns */
    int score;
    float screen_timer; /* time on the current screen, drives blinking */
} Game;

/* Initialize the game to the title screen. */
void game_init(Game *g);

/* Advance the current screen (input, physics, state transitions). */
void game_update(Game *g, float dt);

/* Render everything for the current screen. */
void game_draw(const Game *g);

#endif