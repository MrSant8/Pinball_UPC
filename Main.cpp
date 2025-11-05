#include "raylib.h"
#include "Physics.h"
#include "Game.h"

int main() {
    InitWindow(377, 655, "Pinball");

    SetTargetFPS(120);

    Physics physics;
    Game game(&physics);

    while (!WindowShouldClose()) {
        game.Update();

        BeginDrawing();
        ClearBackground(BLACK);
        game.Draw();
        EndDrawing();
    }

    CloseWindow();
    return 0;
}