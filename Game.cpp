#include "Game.h"
#include "Units.h"
#include "raylib.h"

static inline float RadToDeg(float r) { return r * (180.0f / 3.14159265f); }

Game::Game(Physics* p) : physics(p) {
    // === Cargar imagen de fondo ===
    // Nnombre es correcto (mapa.png)
    fondo = LoadTexture("Assets/mapa.png");

 
// === Paredes del Pinball (mapa_coords.txt) ===
    {
        // Tamaño del array 142
  int wallCoords[198] = {
	358, 655,
	358, 448,
	358, 412,
	358, 171,
	356, 157,
	354, 145,
	352, 135,
	350, 129,
	346, 119,
	344, 114,
	342, 110,
	338, 101,
	336, 97,
	331, 89,
	328, 85,
	324, 82,
	320, 77,
	316, 73,
	310, 67,
	304, 62,
	299, 58,
	293, 52,
	285, 47,
	266, 38,
	253, 32,
	243, 29,
	219, 24,
	204, 22,
	173, 22,
	156, 25,
	146, 26,
	135, 29,
	128, 32,
	115, 37,
	110, 39,
	104, 41,
	98, 44,
	89, 49,
	83, 52,
	79, 56,
	75, 60,
	70, 64,
	64, 71,
	59, 76,
	55, 80,
	49, 87,
	41, 100,
	37, 109,
	29, 123,
	24, 143,
	21, 160,
	20, 211,
	21, 221,
	23, 231,
	34, 263,
	42, 285,
	71, 365,
	78, 379,
	77, 385,
	74, 389,
	84, 398,
	84, 405,
	44, 445,
	38, 445,
	26, 436,
	26, 591,
	33, 599,
	44, 599,
	50, 593,
	57, 593,
	104, 619,
	104, 654,
	236, 654,
	236, 618,
	289, 591,
	294, 591,
	301, 599,
	313, 599,
	320, 591,
	320, 467,
	288, 452,
	288, 447,
	320, 359,
	320, 197,
	307, 184,
	301, 184,
	288, 197,
	273, 197,
	240, 164,
	240, 115,
	267, 87,
	282, 87,
	308, 113,
	318, 137,
	322, 137,
	330, 152,
	332, 161,
	332, 655,
	358, 655
};

        // Vértices a 71 (142 / 2)
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
    palancaIzquierda.w = 50.0f;  palancaIzquierda.h = 10.0f;
    palancaDerecha.w = 50.0f;  palancaDerecha.h = 10.0f;

    // Coordenadas de los flippers para que encajen en el mapa
    float yFlipper = 605.0f;//num PETIT -> flippers DALT, num GRAN -> flippers BAIX
    float xLeft = 131.0f;    //num GRAN -> DRETA
    float xRight = 209.0f;   //num PETIT -> ESQUERRA

    palancaIzquierda.body = physics->CreateBox(xLeft, yFlipper, palancaIzquierda.w, palancaIzquierda.h, true);
    palancaDerecha.body = physics->CreateBox(xRight, yFlipper, palancaDerecha.w, palancaDerecha.h, true);

    palancaIzquierda.body->SetAngularDamping(5.0f);
    palancaDerecha.body->SetAngularDamping(5.0f);

    float leftAnchorX = xLeft - palancaIzquierda.w * 0.5f + 6.0f;
    float rightAnchorX = xRight + palancaDerecha.w * 0.5f - 6.0f;

    float lower = -30.0f, upper = 30.0f;

    // Se usa 'flipperAnchor' en lugar de 'ground.body'
    leftJoint = physics->CreateRevoluteJoint(
        flipperAnchor, palancaIzquierda.body, leftAnchorX, yFlipper, lower, upper, true, -8.0f, 120.0f);
    rightJoint = physics->CreateRevoluteJoint(
        flipperAnchor, palancaDerecha.body, rightAnchorX, yFlipper, lower, upper, true, 8.0f, 120.0f);


    
    

    // === Rampas ===
    rampLeft.w = 120.0f; rampLeft.h = 12.0f;
    rampLeft.body = physics->CreateBox(720.0f, 100.0f, rampLeft.w, rampLeft.h, false);
    rampLeft.body->SetTransform(rampLeft.body->GetPosition(), 45);

    rampRight.w = 120.0f; rampRight.h = 12.0f;
    rampRight.body = physics->CreateBox(540.0f, 200.0f, rampRight.w, rampRight.h, false);

    // === Postes ===
    posts[0] = physics->CreateCircle(270.0f, 320.0f, postRadius, false, 0.0f, 0.3f, 0.9f);
    posts[1] = physics->CreateCircle(330.0f, 280.0f, postRadius, false, 0.0f, 0.3f, 0.9f);
    posts[2] = physics->CreateCircle(470.0f, 280.0f, postRadius, false, 0.0f, 0.3f, 0.9f);
    posts[3] = physics->CreateCircle(530.0f, 320.0f, postRadius, false, 0.0f, 0.3f, 0.9f);

    // === Bumpers ===
    bumpers[0] = physics->CreateCircle(380.0f, 160.0f, bumperRadius, false, 0.0f, 0.3f, 1.0f);
    bumpers[1] = physics->CreateCircle(450.0f, 240.0f, bumperRadius, false, 0.0f, 0.3f, 1.0f);
    bumpers[2] = physics->CreateCircle(310.0f, 230.0f, bumperRadius, false, 0.0f, 0.3f, 1.0f);
}

Game::~Game() {
    UnloadTexture(fondo);
}

void Game::Update() {
    // Colocar bola en el carril
    if (IsKeyPressed(KEY_B)) {
        ball->SetTransform(b2Vec2(PIXEL_TO_METERS(340.0f), PIXEL_TO_METERS(600.0f)), 0.0f);
        ball->SetLinearVelocity(b2Vec2_zero);
        ball->SetAngularVelocity(0.0f);
        ball->SetAwake(true);
    }

    // Lanzar bola (espacio)
    if (IsKeyPressed(KEY_SPACE)) {
        b2Vec2 pos = ball->GetPosition();
        float x = METERS_TO_PIXELS(pos.x);
        float y = METERS_TO_PIXELS(pos.y);

        bool enCarril = (x > 332.0f && x < 358.0f && y > 400.0f);

        if (enCarril)
            ball->ApplyLinearImpulseToCenter(b2Vec2(0.0f, -6.0f), true);
    }

    // Reset de bola
    if (IsKeyPressed(KEY_R)) {
        ball->SetTransform(b2Vec2(PIXEL_TO_METERS(340.0f), PIXEL_TO_METERS(600.0f)), 0.0f);
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

    DrawBoxRot(palancaIzquierda, RED);
    DrawBoxRot(palancaDerecha, RED);




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