#pragma once

// Silenciamos la advertencia 26495 de miembros no inicializados al incluir Box2D (solo afecta a este header)
#pragma warning(push)
#pragma warning(disable : 26495)
#include "box2d/box2d.h"
#pragma warning(pop)

class Physics {
public:
    Physics();
    ~Physics();

    // Avanza la simulación (llamar una vez por frame)
    void Step(float dt = 1.0f / 60.0f, int velIters = 8, int posIters = 3);

    // Crea una caja centrada en (x,y) con tamaño w x h en píxeles
    b2Body* CreateBox(float x, float y, float w, float h, bool dynamic);

    // Crea un círculo centrado en (x,y) con radio en píxeles
    b2Body* CreateCircle(float x, float y, float radius, bool dynamic,
        float density = 1.0f, float friction = 0.3f, float restitution = 0.8f);

    // Crea una bisagra entre bodyA y bodyB en un ancla del MUNDO (en píxeles)
    // lower/upper en grados. Motor opcional (velocidad en rad/s, par máximo).
    b2RevoluteJoint* CreateRevoluteJoint(
        b2Body* bodyA, b2Body* bodyB,
        float anchorX_px, float anchorY_px,
        float lowerDeg, float upperDeg,
        bool enableMotor, float motorSpeedRad, float maxMotorTorque
    );

    // Acceso al mundo (por si lo necesitas)
    b2World* GetWorld() const { return world; }

private:
    b2World* world = nullptr;
};
