#pragma once
#include "Scene.h"

class Player {
public:
    explicit Player(Scene* scene);
    void HandleInput();
private:
    Scene* scene = nullptr;
    const float flipperMotorSpeedUp = -25.0f;
    const float flipperMotorSpeedDown = 15.0f;
};