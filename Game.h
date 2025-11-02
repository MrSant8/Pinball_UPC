#pragma once
#include "Physics.h"
#include "raylib.h"

// Caja "dibujable"
struct BoxSprite {
    b2Body* body = nullptr;
    float w = 0.0f;
    float h = 0.0f;
};

class Game {
public:
    explicit Game(Physics* physics);
    ~Game(); // destructor para descargar textura

    void Update();
    void Draw();

private:
    Physics* physics = nullptr;

    // Fondo
    Texture2D fondo;

    // Bola
    b2Body* ball = nullptr;
    float ballRadius = 10.0f;

    // Flippers
    BoxSprite palancaIzquierda;
    BoxSprite palancaDerecha;
    b2RevoluteJoint* leftJoint = nullptr;
    b2RevoluteJoint* rightJoint = nullptr;
    b2Body* flipperAnchor = nullptr; 

    // Rampas
    BoxSprite rampLeft;
    BoxSprite rampRight;

    // Postes y bumpers
    static const int kNumPosts = 4;
    b2Body* posts[kNumPosts];
    float postRadius = 10.0f;

    static const int kNumBumpers = 3;
    b2Body* bumpers[kNumBumpers];
    float bumperRadius = 18.0f;

    // Dibujo auxiliar
    void DrawBoxAA(const BoxSprite& bx, Color c);
    void DrawBoxRot(const BoxSprite& bx, Color c);
};