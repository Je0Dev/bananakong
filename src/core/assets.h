#ifndef ASSETS_H
#define ASSETS_H

#include "raylib.h"

/* Sound effects playable by id; loaded once at startup. */
typedef enum SoundId {
    SND_JUMP,
    SND_THROW,
    SND_COIN,
    SND_HURT,
    SND_SELECT,
    SND_GEM,
    SND_BUMP,
    SND_COUNT
} SoundId;

/* All game textures and sounds, loaded once before the main loop. */
typedef struct Assets {
    Texture2D player_idle;
    Texture2D player_walk_a;
    Texture2D player_walk_b;
    Texture2D player_jump;
    Texture2D player_climb_a;
    Texture2D player_climb_b;
    Texture2D player_hit;
    Texture2D player_duck;
    Texture2D tile_grass;
    Texture2D ladder_top;
    Texture2D ladder_middle;
    Texture2D ladder_bottom;
    Texture2D flag_a;
    Texture2D flag_b;
    Texture2D background;
    Texture2D bomb;
    Sound sounds[SND_COUNT];
} Assets;

/* Load every texture and sound from the assets/ folder. */
void assets_load(Assets *a);

/* Free all textures and sounds (reverse of assets_load). */
void assets_unload(Assets *a);

/* Play a sound effect, restarting it if already playing. */
void assets_play(const Assets *a, SoundId id);

#endif