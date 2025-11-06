#include "Game.h"
#include "Units.h"
#include "raylib.h"

static inline float RadToDeg(float r) { return r * (180.0f / 3.14159265f); }

Game::Game(Physics* p) : physics(p) {

    fondo = LoadTexture("Assets/mapaHD.png");

    bumperIzquierdoTexture = LoadTexture("Assets/BumperIzquierdaPeque�o.png");
    bumperDerechoTexture = LoadTexture("Assets/BumperDerechoPeque�o.png"); 
    
    
    {
    int BumperIzquierdaPeque�o[24] = {
    86, 235,
    93, 255,
    92, 260,
    89, 263,
    84, 264,
    78, 254,
    73, 242,
    67, 224,
    67, 218,
    72, 214,
    76, 214,
    86, 233
    };
    
    const int numVertices = 12;

    b2Vec2 wallVertices[numVertices];
    for (int i = 0; i < numVertices; ++i) {
        wallVertices[i].Set((float)BumperIzquierdaPeque�o[i * 2], (float)BumperIzquierdaPeque�o[i * 2 + 1]);
    }

    physics->CreateChain(wallVertices, numVertices, 0.3f, 0.1f);
    }
    
    {
        int BumperDerechoPeque�o[18] = {
        296, 248,
        296, 331,
        288, 339,
        282, 339,
        274, 331,
        274, 247,
        281, 239,
        288, 239,
        296, 245
        };

        const int numVertices = 12;

        b2Vec2 wallVertices[numVertices];
        for (int i = 0; i < numVertices; ++i) {
            wallVertices[i].Set((float)BumperDerechoPeque�o[i * 2], (float)BumperDerechoPeque�o[i * 2 + 1]);
        }

        physics->CreateChain(wallVertices, numVertices, 0.3f, 0.1f);
    }
    
    {
        int wallCoords[200] = {
            358, 654,
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
            237, 654,
            237, 618,
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
            332, 162,
            332, 412,
            332, 655,
            346, 655,
            362, 655
        };
   
        const int numVertices = 100;

        b2Vec2 wallVertices[numVertices];
        for (int i = 0; i < numVertices; ++i) {
            wallVertices[i].Set((float)wallCoords[i * 2], (float)wallCoords[i * 2 + 1]);
        }

        physics->CreateChain(wallVertices, numVertices, 0.3f, 0.1f);
    }

    flipperAnchor = physics->CreateBox(1, 1, 1, 1, false);


    // === Bola ===

    ballRadius = 7.0f;
    ball = physics->CreateCircle(345.0f, 600.0f, ballRadius, true, 1.0f, 0.2f, 0.8f);

    // === Flippers ===

    palancaIzquierda.w = 50.0f;  
    palancaIzquierda.h = 10.0f;
    palancaDerecha.w = 50.0f;  
    palancaDerecha.h = 10.0f;

    float yFlipper = 605.0f; //num PETIT -> flippers DALT, num GRAN -> flippers BAIX
    float xLeft = 131.0f;    //num GRAN -> DRETA
    float xRight = 209.0f;   //num PETIT -> ESQUERRA

    palancaIzquierda.body = physics->CreateBox(xLeft, yFlipper, palancaIzquierda.w, palancaIzquierda.h, true);
    palancaDerecha.body = physics->CreateBox(xRight, yFlipper, palancaDerecha.w, palancaDerecha.h, true);

    palancaIzquierda.body->SetAngularDamping(5.0f);
    palancaDerecha.body->SetAngularDamping(5.0f);

    float leftAnchorX = xLeft - palancaIzquierda.w * 0.5f + 6.0f;
    float rightAnchorX = xRight + palancaDerecha.w * 0.5f - 6.0f;

    float lower = -30.0f, upper = 30.0f;

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


    // === Poste Vertial ===

    posteVertical1.w = 20.0f;
    posteVertical1.h = 90.0f;
    posteVertical1.body = physics->CreateBox(200.0f, 310.0f, posteVertical1.w, posteVertical1.h, false);

    // Poste derecha medio del mapa ( el que tiene agujero arriba pegado ), falta editar para que se parecza a un 0
    // color blanco editar abajo en DRAW para eliminar el color y dejar el box coollider
    
    // =========================================
    // Editar linea 300 pa borrar los colores
    // =========================================

    posteVertical5.w = 20.0f;
    posteVertical5.h = 90.0f;
    posteVertical5.body = physics->CreateBox(73.0f, 283.0f, posteVertical5.w, posteVertical5.h, false);

    // === Postes 3 juntos arriba ===
    posteVertical2.w = 8.0f;
    posteVertical2.h = 50.0f;
    posteVertical2.body = physics->CreateBox(138.8f, 140.0f, posteVertical2.w, posteVertical2.h, false);


    posteVertical3.w = 8.0f;
    posteVertical3.h = 50.0f;
    posteVertical3.body = physics->CreateBox(174.8f, 140.0f, posteVertical3.w, posteVertical2.h, false);


    posteVertical4.w = 8.0f;
    posteVertical4.h = 50.0f;
    posteVertical4.body = physics->CreateBox(210.8f, 140.0f, posteVertical4.w, posteVertical4.h, false);



 
    

    // === Postes ===

    posts[0] = physics->CreateCircle(270.0f, 320.0f, postRadius, false, 0.0f, 0.3f, 0.9f);
    posts[1] = physics->CreateCircle(330.0f, 280.0f, postRadius, false, 0.0f, 0.3f, 0.9f);
    posts[2] = physics->CreateCircle(470.0f, 280.0f, postRadius, false, 0.0f, 0.3f, 0.9f);
    posts[3] = physics->CreateCircle(530.0f, 320.0f, postRadius, false, 0.0f, 0.3f, 0.9f);

    // === Circulitos ===

    float centerX = 377.0f / 2.0f; 
    bumpers[0] = physics->CreateCircle(173.0f, 219.4f, bumperRadius, false, 0.0f, 0.3f, 1.0f); // Bumper arriba, no tocar en teoria esta bien :)
    bumpers[1] = physics->CreateCircle(126.0f, 295.5f, bumperRadius, false, 0.0f, 0.3f, 1.0f); // Bumper izquierdo, no tocar en teoria esta bien :)
    bumpers[2] = physics->CreateCircle(220.0f, 295.5f, bumperRadius, false, 0.0f, 0.3f, 1.0f); // Bumper derecho, no tocar en teoria esta bien :)
    bumpers[0]->SetType(b2_kinematicBody);
    bumpers[1]->SetType(b2_kinematicBody);
    bumpers[2]->SetType(b2_kinematicBody);
}

Game::~Game() 
{
    UnloadTexture(fondo);
    UnloadTexture(bumperIzquierdoTexture);
    UnloadTexture(bumperDerechoTexture);
}

void Game::Update() 
{
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
            ball->ApplyLinearImpulseToCenter(b2Vec2(0.0f, -40.0f), true);
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

void Game::OnCollision(b2Body* bodyA, b2Body* bodyB)
{
    score += 100;
    
    //Audio

    if (bodyA->GetType() == b2_kinematicBody) {
        score += 1000;
    }
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

// --- Funci�n Principal de Dibujo ---

void Game::Draw() {
    DrawTexture(fondo, 0, 0, WHITE);

    // Draw physical objects with debug colors
    DrawBoxRot(palancaIzquierda, BLUE);
    DrawBoxRot(palancaDerecha, BLUE);

    // Draw bumpers with debug colors
    DrawTexture(bumperIzquierdoTexture, 0, 0, WHITE);
    DrawTexture(bumperDerechoTexture, 0, 0, WHITE);

    // Draw posts with debug colors
    DrawBoxAA(posteVertical2, YELLOW);
    DrawBoxAA(posteVertical3, YELLOW);
    DrawBoxAA(posteVertical4, YELLOW);

    // Draw ramps with debug colors
    DrawBoxAA(rampLeft, ORANGE);
    DrawBoxAA(rampRight, ORANGE);

    // Draw wall colliders (from the wallCoords array)
    // This is a simplified visualization - you might want to draw the actual chain shape
    // For now, we'll just draw the first and last points
    b2Vec2 wallVertices[100];
    for (int i = 0; i < 100; ++i) {
        wallVertices[i].Set((float)wallCoords[i * 2], (float)wallCoords[i * 2 + 1]);
    }
    for (int i = 0; i < 99; ++i) {
        DrawLineV(
            {wallVertices[i].x, wallVertices[i].y},
            {wallVertices[i+1].x, wallVertices[i+1].y},
            PURPLE
        );
    }

    // Draw ball
    b2Vec2 pb = ball->GetPosition();
    DrawCircle((int)pb.x, (int)pb.y, ballRadius, WHITE);

    // Debug UI
    DrawFPS(10, 10);
    DrawTextEx(GetFontDefault(), TextFormat("Score: %d", score), { 10, 30 }, 20, 1, YELLOW);
    DrawTextEx(GetFontDefault(), TextFormat("HighScore: %d", highScore), { 10, 60 }, 20, 1, YELLOW);
    
    // Draw collider legend
    DrawText("Colliders:", 10, 90, 16, WHITE);
    DrawRectangle(10, 110, 10, 10, RED); DrawText("Left Bumper", 25, 110, 12, WHITE);
    DrawRectangle(10, 125, 10, 10, GREEN); DrawText("Right Bumper", 25, 125, 12, WHITE);
    DrawRectangle(10, 140, 10, 10, BLUE); DrawText("Flippers", 25, 140, 12, WHITE);
    DrawRectangle(10, 155, 10, 10, YELLOW); DrawText("Posts", 25, 155, 12, WHITE);
    DrawRectangle(10, 170, 10, 10, ORANGE); DrawText("Ramps", 25, 170, 12, WHITE);
    DrawRectangle(10, 185, 10, 10, PURPLE); DrawText("Map Walls", 25, 185, 12, WHITE);
}



void Game::ScoreRefresh() 
{
    pastScore = score;
    if (score > highScore)
    {
        highScore = score;
    }
    score = 0;
}

