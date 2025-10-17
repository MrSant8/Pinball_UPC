#include "raylib.h"
#include "Physics.h"
#include "Game.h"

int main() {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);   // <- opcional
    InitWindow(800, 1000, "Pinball - Fondo con mapa"); //800-1000
    SetTargetFPS(165);

    Physics physics;
    Game game(&physics);

    while (!WindowShouldClose()) {
        game.Update();

        BeginDrawing();
        ClearBackground(BLACK);  // mejor negro si tu imagen no cubre por alguna razón
        game.Draw();
        EndDrawing();
    }

    CloseWindow();
    return 0;
}


