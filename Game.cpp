#include "Game.h"
#include "Units.h"
#include "raylib.h"

static inline float RadToDeg(float r) { return r * (180.0f / 3.14159265f); }

Game::Game(Physics* p) : physics(p) {
    // --- Suelo ---
    ground.w = 800.0f;  ground.h = 20.0f;
    ground.body = physics->CreateBox(400.0f, 590.0f, ground.w, ground.h, false);

    // --- Paredes ---
    leftWall.w = 20.0f;  leftWall.h = 600.0f;
    leftWall.body = physics->CreateBox(10.0f, 300.0f, leftWall.w, leftWall.h, false);

    rightWall.w = 20.0f; rightWall.h = 600.0f;
    rightWall.body = physics->CreateBox(790.0f, 300.0f, rightWall.w, rightWall.h, false);

    // --- Carril derecho (lanzador) ---
    laneInner.w = 8.0f;   laneInner.h = 560.0f; laneInner.body = physics->CreateBox(760.0f, 300.0f, laneInner.w, laneInner.h, false);
    laneOuter.w = 8.0f;   laneOuter.h = 560.0f; laneOuter.body = physics->CreateBox(785.0f, 300.0f, laneOuter.w, laneOuter.h, false);

    // --- Bola (círculo físico real con alta restitución) ---
    ballRadius = 10.0f;
    ball = physics->CreateCircle(772.0f, 560.0f, ballRadius, true, 1.0f, 0.2f, 0.8f);

    // --- Flippers ---
    palancaIzquierda.w = 80.0f;  palancaIzquierda.h = 18.0f;
    palancaDerecha.w = 80.0f;  palancaDerecha.h = 18.0f;

    float yFlipper = 540.0f;
    float xLeft = 300.0f;
    float xRight = 500.0f;

    palancaIzquierda.body = physics->CreateBox(xLeft, yFlipper, palancaIzquierda.w, palancaIzquierda.h, true);
    palancaDerecha.body = physics->CreateBox(xRight, yFlipper, palancaDerecha.w, palancaDerecha.h, true);
    palancaIzquierda.body->SetAngularDamping(5.0f);
    palancaDerecha.body->SetAngularDamping(5.0f);

    float leftAnchorX = xLeft - palancaIzquierda.w * 0.5f + 6.0f;
    float leftAnchorY = yFlipper;
    float rightAnchorX = xRight + palancaDerecha.w * 0.5f - 6.0f;
    float rightAnchorY = yFlipper;

    float lower = -30.0f, upper = 30.0f;
    float motorSpeedIdleLeft = -8.0f;
    float motorSpeedIdleRight = 8.0f;
    float motorTorque = 120.0f;

    leftJoint = physics->CreateRevoluteJoint(
        ground.body, palancaIzquierda.body,
        leftAnchorX, leftAnchorY,
        lower, upper,
        true, motorSpeedIdleLeft, motorTorque
    );

    rightJoint = physics->CreateRevoluteJoint(
        ground.body, palancaDerecha.body,
        rightAnchorX, rightAnchorY,
        lower, upper,
        true, motorSpeedIdleRight, motorTorque
    );

    // --- Rampas simples (cajas estáticas) ---
    // Nota: por simplicidad las dejo sin rotar (AA). Si quieres inclinarlas,
    // cambiamos a cuerpos dinámicos + setTransform o usamos polígonos con SetAsBox con ángulo.
    rampLeft.w = 120.0f; rampLeft.h = 12.0f;  rampLeft.body = physics->CreateBox(220.0f, 200.0f, rampLeft.w, rampLeft.h, false);
    rampRight.w = 120.0f; rampRight.h = 12.0f; rampRight.body = physics->CreateBox(540.0f, 200.0f, rampRight.w, rampRight.h, false);

    // --- Postes (cilindros pequeños) ---
    posts[0] = physics->CreateCircle(270.0f, 320.0f, postRadius, false, 0.0f, 0.3f, 0.9f);
    posts[1] = physics->CreateCircle(330.0f, 280.0f, postRadius, false, 0.0f, 0.3f, 0.9f);
    posts[2] = physics->CreateCircle(470.0f, 280.0f, postRadius, false, 0.0f, 0.3f, 0.9f);
    posts[3] = physics->CreateCircle(530.0f, 320.0f, postRadius, false, 0.0f, 0.3f, 0.9f);

    // --- Bumpers (más grandes, súper rebote) ---
    bumpers[0] = physics->CreateCircle(380.0f, 160.0f, bumperRadius, false, 0.0f, 0.3f, 1.0f);
    bumpers[1] = physics->CreateCircle(450.0f, 240.0f, bumperRadius, false, 0.0f, 0.3f, 1.0f);
    bumpers[2] = physics->CreateCircle(310.0f, 230.0f, bumperRadius, false, 0.0f, 0.3f, 1.0f);
}

void Game::Update() {
    // Colocar bola al carril (B)
    if (IsKeyPressed(KEY_B)) {
        ball->SetTransform(b2Vec2(PIXEL_TO_METERS(772.0f), PIXEL_TO_METERS(560.0f)), 0.0f);
        ball->SetLinearVelocity(b2Vec2_zero);
        ball->SetAngularVelocity(0.0f);
        ball->SetAwake(true);
    }

    // Lanzar bola (ESPACIO)
    if (IsKeyPressed(KEY_SPACE)) {
        // Si está en el carril (aproximación por zona)
        b2Vec2 pos = ball->GetPosition();
        float x = METERS_TO_PIXELS(pos.x);
        float y = METERS_TO_PIXELS(pos.y);
        bool enCarril = (x > 750.0f && x < 790.0f && y > 480.0f);
        if (enCarril) {
            ball->ApplyLinearImpulseToCenter(b2Vec2(0.0f, -6.0f), true); // impulso vertical
        }
    }

    // Reset rápido de bola al área alta (R)
    if (IsKeyPressed(KEY_R)) {
        ball->SetTransform(b2Vec2(PIXEL_TO_METERS(400.0f), PIXEL_TO_METERS(120.0f)), 0.0f);
        ball->SetLinearVelocity(b2Vec2_zero);
        ball->SetAngularVelocity(0.0f);
    }

    // Flippers: pulsar = subir, soltar = bajar
    if (IsKeyDown(KEY_LEFT))  leftJoint->SetMotorSpeed(-12.0f); else leftJoint->SetMotorSpeed(8.0f);
    if (IsKeyDown(KEY_RIGHT)) rightJoint->SetMotorSpeed(12.0f); else rightJoint->SetMotorSpeed(-8.0f);

    physics->Step();
}

void Game::DrawBoxAA(const BoxSprite& bx, Color c) {
    // Dibujo no-rotado (suelo, paredes, carril, rampas AA)
    b2Vec2 p = bx.body->GetPosition();
    float px = METERS_TO_PIXELS(p.x);
    float py = METERS_TO_PIXELS(p.y);
    DrawRectangle((int)(px - bx.w * 0.5f), (int)(py - bx.h * 0.5f), (int)bx.w, (int)bx.h, c);
}

void Game::DrawBoxRot(const BoxSprite& bx, Color c) {
    // Dibujo rotado (flippers)
    b2Vec2 p = bx.body->GetPosition();
    float px = METERS_TO_PIXELS(p.x);
    float py = METERS_TO_PIXELS(p.y);
    float angleDeg = RadToDeg(bx.body->GetAngle());
    Rectangle rect = { px, py, bx.w, bx.h };
    Vector2 origin = { bx.w * 0.5f, bx.h * 0.5f };
    DrawRectanglePro(rect, origin, angleDeg, c);
}

void Game::Draw() {
    // Suelo y paredes
    DrawBoxAA(ground, DARKGRAY);
    DrawBoxAA(leftWall, DARKGRAY);
    DrawBoxAA(rightWall, DARKGRAY);

    // Carril derecho (lanzador)
    DrawBoxAA(laneInner, GRAY);
    DrawBoxAA(laneOuter, GRAY);

    // Rampas (planas por ahora)
    DrawBoxAA(rampLeft, LIGHTGRAY);
    DrawBoxAA(rampRight, LIGHTGRAY);

    // Flippers
    DrawBoxRot(palancaIzquierda, BLUE);
    DrawBoxRot(palancaDerecha, BLUE);

    // Bola
    b2Vec2 pb = ball->GetPosition();
    DrawCircle((int)METERS_TO_PIXELS(pb.x), (int)METERS_TO_PIXELS(pb.y), ballRadius, RED);

    // Postes
    for (int i = 0; i < kNumPosts; ++i) {
        if (!posts[i]) continue;
        b2Vec2 p = posts[i]->GetPosition();
        DrawCircleLines((int)METERS_TO_PIXELS(p.x), (int)METERS_TO_PIXELS(p.y), postRadius, DARKGREEN);
        DrawCircle((int)METERS_TO_PIXELS(p.x), (int)METERS_TO_PIXELS(p.y), postRadius - 3.0f, GREEN);
    }

    // Bumpers
    for (int i = 0; i < kNumBumpers; ++i) {
        if (!bumpers[i]) continue;
        b2Vec2 p = bumpers[i]->GetPosition();
        DrawCircleLines((int)METERS_TO_PIXELS(p.x), (int)METERS_TO_PIXELS(p.y), bumperRadius, MAROON);
        DrawCircle((int)METERS_TO_PIXELS(p.x), (int)METERS_TO_PIXELS(p.y), bumperRadius - 4.0f, ORANGE);
    }

    DrawText("B: bola al carril  |  ESPACIO: lanzar  |  LEFT/RIGHT: flippers  |  R: reset", 8, 8, 16, GRAY);
}
