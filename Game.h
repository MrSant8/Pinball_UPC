#pragma once
#include "Physics.h"
#include "raylib.h"   // para Color / dibujo

// Caja "dibujable"
struct BoxSprite {
    b2Body* body = nullptr;
    float w = 0.0f; // píxeles
    float h = 0.0f; // píxeles
};

class Game {
public:
    explicit Game(Physics* physics);
    void Update();
    void Draw();

private:
    Physics* physics = nullptr;

    // Base
    BoxSprite ground;
    BoxSprite leftWall;
    BoxSprite rightWall;

    // Bola (seguimos usando ballRadius para dibujarla)
    b2Body* ball = nullptr;
    float ballRadius = 10.0f;

    // Flippers
    BoxSprite palancaIzquierda;
    BoxSprite palancaDerecha;
    b2RevoluteJoint* leftJoint = nullptr;
    b2RevoluteJoint* rightJoint = nullptr;

    // --- Nuevo: carril derecho (lanzador) ---
    BoxSprite laneInner;
    BoxSprite laneOuter;

    // --- Nuevo: obstáculos ---
    // Rampas simples (cajas estáticas, podemos rotarlas a futuro)
    BoxSprite rampLeft;
    BoxSprite rampRight;

    // Postes/cilindros
    static const int kNumPosts = 4;
    b2Body* posts[kNumPosts] = { nullptr, nullptr, nullptr, nullptr };
    float   postRadius = 10.0f;

    // Bumpers (más grandes y con mucha restitución)
    static const int kNumBumpers = 3;
    b2Body* bumpers[kNumBumpers] = { nullptr, nullptr, nullptr };
    float   bumperRadius = 18.0f;

    void DrawBoxAA(const BoxSprite& bx, Color c);  // eje-alineado
    void DrawBoxRot(const BoxSprite& bx, Color c); // rotado
};
