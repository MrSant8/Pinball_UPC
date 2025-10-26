#include "Game.h"
#include "Units.h"
#include "raylib.h"

static inline float RadToDeg(float r) { return r * (180.0f / 3.14159265f); }

Game::Game(Physics* p) : physics(p) {
    // === Cargar imagen de fondo ===
    fondo = LoadTexture("Assets/mapa.png");  // asegúrate de que exista esta ruta

    // === Suelo y paredes ===
    ground.w = 1500.0f; ground.h = 20.0f;
    ceiling.w = 1500.0f; ceiling.h = 20.0f;
    ground.body = physics->CreateBox(100.0f, 1000.0f, ground.w, ground.h, false);//625

    ceiling.body = physics->CreateBox(100.0f, 10.0f, ceiling.w, ceiling.h, false);

    leftWall.w = 20.0f;  leftWall.h = 1000.0f;
    leftWall.body = physics->CreateBox(10.0f, 500.0f, leftWall.w, leftWall.h, false);

    rightWall.w = 20.0f; rightWall.h = 1000.0f;
    rightWall.body = physics->CreateBox(790.0f, 500.0f, rightWall.w, rightWall.h, false);

    b2Vec2* mapCoords[78] = {
    (1, 0,
    0, 634,
    104, 633,
    105, 609,
    56, 582,
    50, 582,
    44, 587,
    35, 588,
    26, 580,
    26, 426,
    77, 375,
    79, 366,
    21, 202,
    21, 158,
    28, 113,
    40, 93,
    48, 77,
    84, 41,
    102, 32,
    122, 23,
    134, 18,
    150, 14,
    164, 11,
    174, 11,
    206, 10,
    223, 13,
    242, 16,
    260, 23,
    273, 30,
    286, 38,
    295, 43,
    328, 76,
    348, 112,
    353, 132,
    357, 148,
    358, 160,
    358, 632,
    377, 633,
    376, 0)
    };

    
    //b2Vec2 mapVector;
    //mapVector.
    //mapMain.body = physics->CreatePolygon(mapCoords);

    //NO VA JAJA LOL ME CAGO EN TODO

    // === Carril derecho (lanzador) ===
    laneInner.w = 8.0f;   laneInner.h = 800.0f;
    laneOuter.w = 8.0f;   laneOuter.h = 800.0f;
    laneInner.body = physics->CreateBox(750.0f, 700.0f, laneInner.w, laneInner.h, false);
    laneOuter.body = physics->CreateBox(785.0f, 700.0f, laneOuter.w, laneOuter.h, false);

    // === Bola ===
    ballRadius = 10.0f;
    ball = physics->CreateCircle(772.0f, 560.0f, ballRadius, true, 1.0f, 0.2f, 0.8f);

    // === Flippers ===
    palancaIzquierda.w = 80.0f;  palancaIzquierda.h = 18.0f;
    palancaDerecha.w = 80.0f;  palancaDerecha.h = 18.0f;

    float yFlipper = 900.0f;
    float xLeft = 250.0f;
    float xRight = 450.0f;

    palancaIzquierda.body = physics->CreateBox(xLeft, yFlipper, palancaIzquierda.w, palancaIzquierda.h, true);
    palancaDerecha.body = physics->CreateBox(xRight, yFlipper, palancaDerecha.w, palancaDerecha.h, true);

    palancaIzquierda.body->SetAngularDamping(5.0f);
    palancaDerecha.body->SetAngularDamping(5.0f);

    float leftAnchorX = xLeft - palancaIzquierda.w * 0.5f + 6.0f;
    float rightAnchorX = xRight + palancaDerecha.w * 0.5f - 6.0f;

    float lower = -30.0f, upper = 30.0f;
    leftJoint = physics->CreateRevoluteJoint(
        ground.body, palancaIzquierda.body, leftAnchorX, yFlipper, lower, upper, true, -8.0f, 120.0f);
    rightJoint = physics->CreateRevoluteJoint(
        ground.body, palancaDerecha.body, rightAnchorX, yFlipper, lower, upper, true, 8.0f, 120.0f);

    // === Rampas ===
    rampLeft.w = 120.0f; rampLeft.h = 12.0f;
    rampLeft.body = physics->CreateBox(720.0f, 100.0f, rampLeft.w, rampLeft.h, false);
    rampLeft.body->SetTransform(rampLeft.body->GetPosition(), 45); //no rota
    //rampLeft.body = physics->CreatePolygon(new b2Vec2(500.0f,500.0f),3,0.3f,0.1f); ------ no se hacer triangulos xd
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
        ball->SetTransform(b2Vec2(PIXEL_TO_METERS(772.0f), PIXEL_TO_METERS(560.0f)), 0.0f);
        ball->SetLinearVelocity(b2Vec2_zero);
        ball->SetAngularVelocity(0.0f);
        ball->SetAwake(true);
    }

    // Lanzar bola (espacio)
    if (IsKeyPressed(KEY_SPACE)) {
        b2Vec2 pos = ball->GetPosition();
        float x = METERS_TO_PIXELS(pos.x);
        float y = METERS_TO_PIXELS(pos.y);
        bool enCarril = (x > 750.0f && x < 790.0f && y > 480.0f);
        if (enCarril)
            ball->ApplyLinearImpulseToCenter(b2Vec2(0.0f, -6.0f), true);
    }

    // Reset de bola
    if (IsKeyPressed(KEY_R)) {
        ball->SetTransform(b2Vec2(PIXEL_TO_METERS(400.0f), PIXEL_TO_METERS(120.0f)), 0.0f);
        ball->SetLinearVelocity(b2Vec2_zero);
        ball->SetAngularVelocity(0.0f);
    }

    // Flippers
    if (IsKeyDown(KEY_LEFT))  leftJoint->SetMotorSpeed(-12.0f); else leftJoint->SetMotorSpeed(8.0f);
    if (IsKeyDown(KEY_RIGHT)) rightJoint->SetMotorSpeed(12.0f); else rightJoint->SetMotorSpeed(-8.0f);

    physics->Step();
}

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

void Game::Draw() {
    // === Fondo estirado ===
    Rectangle src = { 0, 0, (float)fondo.width, (float)fondo.height };
    Rectangle dst = { 0, 0, (float)GetScreenWidth(), (float)GetScreenHeight() };
    Vector2 origin = { 0, 0 };
    DrawTexturePro(fondo, src, dst, origin, 0.0f, WHITE);

    // === Objetos físicos ===
    DrawBoxAA(ground, DARKGRAY);
    DrawBoxAA(ceiling, DARKGRAY);
    DrawBoxAA(leftWall, DARKGRAY);
    DrawBoxAA(rightWall, DARKGRAY);
    DrawBoxAA(laneInner, GRAY);
    //DrawBoxAA(laneOuter, GRAY);
    DrawBoxAA(rampLeft, RED);
    DrawBoxAA(rampRight, LIGHTGRAY);

    DrawBoxRot(palancaIzquierda, BLUE);
    DrawBoxRot(palancaDerecha, BLUE);

    // Bola
    b2Vec2 pb = ball->GetPosition();
    DrawCircle((int)METERS_TO_PIXELS(pb.x), (int)METERS_TO_PIXELS(pb.y), ballRadius, RED);

    // Postes
    for (int i = 0; i < kNumPosts; ++i) {
        if (!posts[i]) continue;
        b2Vec2 p = posts[i]->GetPosition();
        DrawCircle((int)METERS_TO_PIXELS(p.x), (int)METERS_TO_PIXELS(p.y), postRadius, GREEN);
    }

    // Bumpers
    for (int i = 0; i < kNumBumpers; ++i) {
        if (!bumpers[i]) continue;
        b2Vec2 p = bumpers[i]->GetPosition();
        DrawCircle((int)METERS_TO_PIXELS(p.x), (int)METERS_TO_PIXELS(p.y), bumperRadius, ORANGE);
    }

    DrawText("B: bola al carril | ESPACIO: lanzar | LEFT/RIGHT: flippers | R: reset", 8, 8, 16, WHITE);
}
