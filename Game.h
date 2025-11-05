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
    void OnCollision(b2Body* bodyA, b2Body* bodyB);
    void ScoreRefresh();

    int score = 0;
    int highScore = 0;
    int pastScore = 0;

    int remainingBalls = 3;

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

    // test de 0 (como la pelota pero alargado)
	BoxSprite posteVertical1;
    BoxSprite posteVertical2;
    BoxSprite posteVertical3;
    BoxSprite posteVertical4;
    BoxSprite posteVertical5;


    Texture2D bumperIzquierdoTexture;
    b2Body* bumperIzquierdoBody;
    Texture2D bumperDerechoTexture;
	b2Body* bumperDerechoBody;

    // Postes y bumpers
    static const int kNumPosts = 4;
    b2Body* posts[kNumPosts];
    float postRadius = 10.0f;

    static const int kNumBumpers = 3;
    b2Body* bumpers[kNumBumpers];
    float bumperRadius = 24.0f;

    // Dibujo auxiliar
    void DrawBoxAA(const BoxSprite& bx, Color c);
    void DrawBoxRot(const BoxSprite& bx, Color c);
    void DrawBody(b2Body* body, Color color);
};