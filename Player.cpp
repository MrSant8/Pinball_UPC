#include "Player.h"
#include "raylib.h"
#include "Units.h"

Player::Player(Scene* s) : scene(s) {}

void Player::HandleInput() {
    // Control de Flippers
    if (IsKeyDown(KEY_LEFT)) {
        scene->GetLeftFlipperJoint()->SetMotorSpeed(flipperMotorSpeedUp);
    }
    else {
        scene->GetLeftFlipperJoint()->SetMotorSpeed(flipperMotorSpeedDown);
    }
    if (IsKeyDown(KEY_RIGHT)) {
        scene->GetRightFlipperJoint()->SetMotorSpeed(-flipperMotorSpeedUp);
    }
    else {
        scene->GetRightFlipperJoint()->SetMotorSpeed(-flipperMotorSpeedDown);
    }
    // Control del Lanzador
    if (IsKeyPressed(KEY_DOWN)) {
        b2Body* ball = scene->GetBall();
        if (METERS_TO_PIXELS(ball->GetPosition().x) > 730) {
            ball->ApplyLinearImpulseToCenter(b2Vec2(0.0f, -12.0f), true);
        }
    }
}