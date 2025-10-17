#pragma once
#include "Physics.h"
#include "raylib.h"

class Scene {
public:
    explicit Scene(Physics* physics);
    ~Scene();
    void CreateInitialScene();
    void Draw();
    void DrawDebug();
    bool IsBallLost() const;
    void ResetBall();
    b2RevoluteJoint* GetLeftFlipperJoint() const { return leftFlipperJoint; }
    b2RevoluteJoint* GetRightFlipperJoint() const { return rightFlipperJoint; }
    b2Body* GetBall() const { return ball; }

private:
    Physics* physics = nullptr;

    b2Body* leftWall = nullptr;
    b2Body* rightWall = nullptr;
    b2Body* leftSlingshot = nullptr;
    b2Body* rightSlingshot = nullptr;

    b2Body* bumpers[3] = { nullptr };
    float bumperRadius = 22.0f;

    b2Body* ball = nullptr;
    float ballRadius = 10.0f;
    b2Body* leftFlipper = nullptr;
    b2Body* rightFlipper = nullptr;
    b2RevoluteJoint* leftFlipperJoint = nullptr;
    b2RevoluteJoint* rightFlipperJoint = nullptr;

    void DrawBody(b2Body* body, Color color);

    Texture2D backgroundTexture;
};