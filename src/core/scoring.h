#ifndef SCORING_H
#define SCORING_H

#include "game.h"

/* Award points for a barrel rolling off the screen and show a popup. */
void scoring_barrel_popped(Game *g, Vector2 pos);

/* Award the win bonus, persist the best score, and enter the win screen. */
void scoring_player_win(Game *g);

/* Persist the best score and enter the game-over screen. */
void scoring_game_over(Game *g);

#endif