#include "raylib.h"
#include "constants.h"
#include "game.h"

int main(void) {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "bananakong");
    SetTargetFPS(60);

    Game game;
    game_init(&game);

    /* Fixed main loop: update logic, then render the frame. */
    while (!WindowShouldClose()) {
        float dt = GetFrameTime();
        game_update(&game, dt);

        BeginDrawing();
        game_draw(&game);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}