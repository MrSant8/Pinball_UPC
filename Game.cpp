#include "Game.h"
#include "Units.h"
#include "raylib.h"

static inline float RadToDeg(float r) { return r * (180.0f / 3.14159265f); }

Game::Game(Physics* p) : physics(p) {
    // === Cargar imagen de fondo ===
    // <<< Asegúrate de que el nombre es correcto (mapa.jpg o mapa.png)
    fondo = LoadTexture("Assets/mapa.png");

    // === Paredes del Pinball (desde mapa_coords.txt) ===
    // <<< REEMPLAZADO: Todas las paredes viejas (ground, leftWall...) se borran
    // <<< y se reemplazan por este bloque
// === Paredes del Pinball (desde mapa_coords.txt) ===
    {
        // <<< CAMBIADO: Tamaño del array a 142
        int wallCoords[142] = {
            361, 634, 362, 158, 358, 145, 354, 132, 350, 120, 331, 80,
            303, 53, 287, 39, 256, 23, 222, 16, 204, 13, 174, 12,
            136, 18, 105, 31, 87, 41, 65, 61, 51, 75, 44, 83,
            40, 92, 35, 99, 34, 108, 30, 115, 27, 124, 25, 132,
            22, 147, 20, 157, 20, 172, 21, 201, 23, 218, 34, 255,
            54, 307, 77, 368, 73, 379, 83, 388, 84, 395, 45, 433,
            36, 435, 30, 425, 25, 426, 25, 580, 34, 587, 42, 588,
            48, 583, 56, 583, 104, 607, 104, 634, 235, 633, 235, 608,
            291, 580, 301, 588, 311, 590, 320, 581, 318, 456, 303, 448,
            288, 440, 304, 393, 319, 347, 320, 188, 303, 174, 288, 188,
            274, 188, 240, 154, 239, 104, 266, 75, 282, 77, 308, 103,
            319, 127, 330, 152, 330, 634, 359, 635, 363, 160
        };

        // <<< CAMBIADO: Número de vértices a 71 (142 / 2)
        const int numVertices = 71;

        b2Vec2 wallVertices[numVertices];
        for (int i = 0; i < numVertices; ++i) {
            wallVertices[i].Set((float)wallCoords[i * 2], (float)wallCoords[i * 2 + 1]);
        }

        // Esto sigue igual (usa CreateChain)
        physics->CreateChain(wallVertices, numVertices, 0.3f, 0.1f);
    }

    flipperAnchor = physics->CreateBox(1, 1, 1, 1, false);


    // === Bola ===
    ballRadius = 10.0f;

    ball = physics->CreateCircle(345.0f, 600.0f, ballRadius, true, 1.0f, 0.2f, 0.8f);

    // === Flippers ===
    palancaIzquierda.w = 80.0f;  palancaIzquierda.h = 18.0f;
    palancaDerecha.w = 80.0f;  palancaDerecha.h = 18.0f;

    // <<< CAMBIADO: Coordenadas de los flippers para que encajen en el mapa (dentro de 377x635)
    float yFlipper = 585.0f;
    float xLeft = 145.0f;
    float xRight = 195.0f;

    palancaIzquierda.body = physics->CreateBox(xLeft, yFlipper, palancaIzquierda.w, palancaIzquierda.h, true);
    palancaDerecha.body = physics->CreateBox(xRight, yFlipper, palancaDerecha.w, palancaDerecha.h, true);

    palancaIzquierda.body->SetAngularDamping(5.0f);
    palancaDerecha.body->SetAngularDamping(5.0f);

    float leftAnchorX = xLeft - palancaIzquierda.w * 0.5f + 6.0f;
    float rightAnchorX = xRight + palancaDerecha.w * 0.5f - 6.0f;

    float lower = -30.0f, upper = 30.0f;

    // <<< CAMBIADO: Se usa 'flipperAnchor' en lugar de 'ground.body'
    leftJoint = physics->CreateRevoluteJoint(
        flipperAnchor, palancaIzquierda.body, leftAnchorX, yFlipper, lower, upper, true, -8.0f, 120.0f);
    rightJoint = physics->CreateRevoluteJoint(
        flipperAnchor, palancaDerecha.body, rightAnchorX, yFlipper, lower, upper, true, 8.0f, 120.0f);

    // === Rampas ===
    // !!! ADVERTENCIA: Estas coordenadas (720, 100) están fuera de la pantalla (377x635)
    rampLeft.w = 120.0f; rampLeft.h = 12.0f;
    rampLeft.body = physics->CreateBox(720.0f, 100.0f, rampLeft.w, rampLeft.h, false);
    rampLeft.body->SetTransform(rampLeft.body->GetPosition(), 45);

    // !!! ADVERTENCIA: Estas coordenadas (540, 200) están fuera de la pantalla
    rampRight.w = 120.0f; rampRight.h = 12.0f;
    rampRight.body = physics->CreateBox(540.0f, 200.0f, rampRight.w, rampRight.h, false);

    // === Postes ===
    // !!! ADVERTENCIA: Coordenadas (470, 280) y (530, 320) están fuera de la pantalla
    posts[0] = physics->CreateCircle(270.0f, 320.0f, postRadius, false, 0.0f, 0.3f, 0.9f);
    posts[1] = physics->CreateCircle(330.0f, 280.0f, postRadius, false, 0.0f, 0.3f, 0.9f);
    posts[2] = physics->CreateCircle(470.0f, 280.0f, postRadius, false, 0.0f, 0.3f, 0.9f);
    posts[3] = physics->CreateCircle(530.0f, 320.0f, postRadius, false, 0.0f, 0.3f, 0.9f);

    // === Bumpers ===
    // !!! ADVERTENCIA: Coordenadas (380, 160) y (450, 240) están fuera de la pantalla
    bumpers[0] = physics->CreateCircle(380.0f, 160.0f, bumperRadius, false, 0.0f, 0.3f, 1.0f);
    bumpers[1] = physics->CreateCircle(450.0f, 240.0f, bumperRadius, false, 0.0f, 0.3f, 1.0f);
    bumpers[2] = physics->CreateCircle(310.0f, 230.0f, bumperRadius, false, 0.0f, 0.3f, 1.0f);
}

Game::~Game() {
    UnloadTexture(fondo);
}

void Game::Update() {
    // Colocar bola en el carril
    // <<< CAMBIADO: Coordenadas ajustadas a la nueva pantalla
    if (IsKeyPressed(KEY_B)) {
        ball->SetTransform(b2Vec2(PIXEL_TO_METERS(345.0f), PIXEL_TO_METERS(600.0f)), 0.0f);       
        ball->SetLinearVelocity(b2Vec2_zero);
        ball->SetAngularVelocity(0.0f);
        ball->SetAwake(true);
    }

    // Lanzar bola (espacio)
    // <<< CAMBIADO: Lógica del carril ajustada a la nueva pantalla
    if (IsKeyPressed(KEY_SPACE)) {
        b2Vec2 pos = ball->GetPosition();
        float x = METERS_TO_PIXELS(pos.x);
        float y = METERS_TO_PIXELS(pos.y);

        // CAMBIA ESTO:
        // bool enCarril = (x > 290.0f && x < 330.0f && y > 400.0f); // (Esto también estaba mal)
        // POR ESTO (comprueba si está en el carril derecho):
        bool enCarril = (x > 332.0f && x < 358.0f && y > 400.0f);

        if (enCarril)
            ball->ApplyLinearImpulseToCenter(b2Vec2(0.0f, -6.0f), true);
    }

    // Reset de bola
    // <<< CAMBIADO: Coordenadas de reseteo ajustadas (centro-arriba)
    if (IsKeyPressed(KEY_R)) {
        ball->SetTransform(b2Vec2(PIXEL_TO_METERS(188.0f), PIXEL_TO_METERS(120.0f)), 0.0f);
        ball->SetLinearVelocity(b2Vec2_zero);
        ball->SetAngularVelocity(0.0f);
    }

    // Flippers
    if (IsKeyDown(KEY_LEFT))  leftJoint->SetMotorSpeed(-12.0f); else leftJoint->SetMotorSpeed(8.0f);
    if (IsKeyDown(KEY_RIGHT)) rightJoint->SetMotorSpeed(12.0f); else rightJoint->SetMotorSpeed(-8.0f);

    physics->Step();
}

// --- Funciones de Dibujo Auxiliares ---

void Game::DrawBoxAA(const BoxSprite& bx, Color c) {
    b2Vec2 p = bx.body->GetPosition();
    float px = METERS_TO_PIXELS(p.x);
    float py = METERS_TO_PIXELS(p.y);
    DrawRectangle((int)(px - bx.w * 0.5f), (int)(py - bx.h * 0.5f), (int)bx.w, (int)bx.h, c);
}

void Game::DrawBoxRot(const BoxSprite& bx, Color c) {
    b2Vec2 p = bx.body->GetPosition();
    float px = METERS_TO_PIXELS(p.x);
    float py = METERS_TO_PIXELS(p.y);
    float angleDeg = RadToDeg(bx.body->GetAngle());
    Rectangle rect = { px, py, bx.w, bx.h };
    Vector2 origin = { bx.w * 0.5f, bx.h * 0.5f };
    DrawRectanglePro(rect, origin, angleDeg, c);
}

// --- Función Principal de Dibujo ---

void Game::Draw() {
    // === Fondo 1:1 ===
    // <<< CAMBIADO: Se usa DrawTexture (1:1) en lugar de DrawTexturePro (estirado)
    DrawTexture(fondo, 0, 0, WHITE);

    // === Objetos físicos ===
    // <<< ELIMINADO: Dibujo de ground, ceiling, leftWall, rightWall, laneInner, laneOuter

    DrawBoxAA(rampLeft, RED);
    DrawBoxAA(rampRight, LIGHTGRAY);

    // <<< ELIMINADO: Dibujo de palancaIzquierda y palancaDerecha (para que se vea el fondo)

    // Bola
    b2Vec2 pb = ball->GetPosition();
    DrawCircle((int)METERS_TO_PIXELS(pb.x), (int)METERS_TO_PIXELS(pb.y), ballRadius, WHITE);

    DrawText("Development Build 0.29.10", 8, 8, 16, RED);
}