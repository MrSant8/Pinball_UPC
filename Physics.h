#pragma once

#pragma warning(push)
#pragma warning(disable : 26495)
#include "box2d/box2d.h"
#pragma warning(pop)

class Physics {
public:
    Physics();
    ~Physics();

    void Step(float dt = 1.0f / 60.0f, int velIters = 8, int posIters = 3);

    b2Body* CreateBox(float x, float y, float w, float h, bool dynamic);

    b2Body* CreateCircle(float x, float y, float radius, bool dynamic,
        float density = 1.0f, float friction = 0.3f, float restitution = 0.8f);

    b2RevoluteJoint* CreateRevoluteJoint(
        b2Body* bodyA, b2Body* bodyB,
        float anchorX_px, float anchorY_px,
        float lowerDeg, float upperDeg,
        bool enableMotor, float motorSpeedRad, float maxMotorTorque
    );

    // Versión única y correcta de CreatePolygon
    b2Body* CreatePolygon(const b2Vec2* vertices, int count, float friction = 0.3f, float restitution = 0.1f);

    // <<< AÑADIDO: Nueva función para crear cadenas de vértices (evita el error abort())
    b2Body* CreateChain(const b2Vec2* vertices, int count, float friction = 0.3f, float restitution = 0.1f);

    b2World* GetWorld() const { return world; }

private:
    b2World* world = nullptr;
};