#include "raylib.h"
#include "Physics.h"
#include "Game.h"

int main() {
    InitWindow(800, 600, "Pinball desde cero"); // crea ventana
    SetTargetFPS(60);                           // 60 FPS fijos

    Physics physics; // motor de físicas
    Game game(&physics); // nuestro juego

    while (!WindowShouldClose()) { // loop principal
        game.Update();   // avanza física + input
        BeginDrawing();  // empieza frame de raylib
        ClearBackground(RAYWHITE); // limpia pantalla
        game.Draw();     // dibuja escenario + bola
        EndDrawing();    // muestra en ventana
    }

    CloseWindow(); // cerrar ventana
    return 0;
}
