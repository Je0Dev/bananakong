#include <stddef.h>
#include "assets.h"

void assets_load(Assets *a) {
    a->player_idle = LoadTexture("assets/sprites/character_yellow_idle.png");
    a->player_walk_a = LoadTexture("assets/sprites/character_yellow_walk_a.png");
    a->player_walk_b = LoadTexture("assets/sprites/character_yellow_walk_b.png");
    a->player_jump = LoadTexture("assets/sprites/character_yellow_jump.png");
    a->player_climb_a = LoadTexture("assets/sprites/character_yellow_climb_a.png");
    a->player_climb_b = LoadTexture("assets/sprites/character_yellow_climb_b.png");
    a->player_hit = LoadTexture("assets/sprites/character_yellow_hit.png");
    a->player_duck = LoadTexture("assets/sprites/character_yellow_duck.png");
    a->tile_grass = LoadTexture("assets/sprites/terrain_grass_block.png");
    a->ladder_top = LoadTexture("assets/sprites/ladder_top.png");
    a->ladder_middle = LoadTexture("assets/sprites/ladder_middle.png");
    a->ladder_bottom = LoadTexture("assets/sprites/ladder_bottom.png");
    a->flag_a = LoadTexture("assets/sprites/flag_yellow_a.png");
    a->flag_b = LoadTexture("assets/sprites/flag_yellow_b.png");
    a->background = LoadTexture("assets/sprites/background_fade_trees.png");
    a->bomb = LoadTexture("assets/sprites/bomb.png");

    /* Nearest-neighbor keeps the small pixel art crisp when scaled down. */
    Texture2D *textures[] = { &a->player_idle, &a->player_walk_a, &a->player_walk_b,
        &a->player_jump, &a->player_climb_a, &a->player_climb_b, &a->player_hit,
        &a->player_duck, &a->tile_grass, &a->ladder_top, &a->ladder_middle,
        &a->ladder_bottom, &a->flag_a, &a->flag_b, &a->background, &a->bomb };
    for (size_t i = 0; i < sizeof(textures) / sizeof(textures[0]); i++) {
        SetTextureFilter(*textures[i], TEXTURE_FILTER_POINT);
    }

    a->sounds[SND_JUMP] = LoadSound("assets/sounds/sfx_jump.ogg");
    a->sounds[SND_THROW] = LoadSound("assets/sounds/sfx_throw.ogg");
    a->sounds[SND_COIN] = LoadSound("assets/sounds/sfx_coin.ogg");
    a->sounds[SND_HURT] = LoadSound("assets/sounds/sfx_hurt.ogg");
    a->sounds[SND_SELECT] = LoadSound("assets/sounds/sfx_select.ogg");
    a->sounds[SND_GEM] = LoadSound("assets/sounds/sfx_gem.ogg");
    a->sounds[SND_BUMP] = LoadSound("assets/sounds/sfx_bump.ogg");
}

void assets_unload(Assets *a) {
    UnloadTexture(a->player_idle);
    UnloadTexture(a->player_walk_a);
    UnloadTexture(a->player_walk_b);
    UnloadTexture(a->player_jump);
    UnloadTexture(a->player_climb_a);
    UnloadTexture(a->player_climb_b);
    UnloadTexture(a->player_hit);
    UnloadTexture(a->player_duck);
    UnloadTexture(a->tile_grass);
    UnloadTexture(a->ladder_top);
    UnloadTexture(a->ladder_middle);
    UnloadTexture(a->ladder_bottom);
    UnloadTexture(a->flag_a);
    UnloadTexture(a->flag_b);
    UnloadTexture(a->background);
    UnloadTexture(a->bomb);
    for (int i = 0; i < SND_COUNT; i++) UnloadSound(a->sounds[i]);
}

void assets_play(const Assets *a, SoundId id) {
    PlaySound(a->sounds[id]);
}