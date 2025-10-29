#include "raylib.h"
#include "Physics.h"
#include "Game.h"

int main() {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);   // <- opcional

    InitWindow(377, 635, "Pinball");

    SetTargetFPS(165);

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