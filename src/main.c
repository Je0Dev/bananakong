#include "raylib.h"
#include "constants.h"
#include "game.h"
#include "assets.h"

int main(void) {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "bananakong");
    InitAudioDevice();
    SetTargetFPS(60);

    Game game;
    game_init(&game);
    assets_load(&game.assets);

    /* Fixed main loop: update logic, then render the frame. */
    while (!WindowShouldClose()) {
        float dt = GetFrameTime();
        game_update(&game, dt);

        BeginDrawing();
        game_draw(&game);
        EndDrawing();
    }

    assets_unload(&game.assets);
    CloseAudioDevice();
    CloseWindow();
    return 0;
}