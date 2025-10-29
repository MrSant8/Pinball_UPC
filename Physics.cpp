#include "Physics.h"
#include "Units.h"

Physics::Physics() {
    b2Vec2 gravity(0.0f, 9.8f);
    world = new b2World(gravity);
}

Physics::~Physics() {
    delete world;
    world = nullptr;
}

void Physics::Step(float dt, int velIters, int posIters) {
    world->Step(dt, velIters, posIters);
}

b2Body* Physics::CreateBox(float x, float y, float w, float h, bool dynamic) {
    b2BodyDef bd;
    bd.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));
    bd.type = dynamic ? b2_dynamicBody : b2_kinematicBody;
    b2Body* body = world->CreateBody(&bd);

    b2PolygonShape shape;
    shape.SetAsBox(PIXEL_TO_METERS(w * 0.5f), PIXEL_TO_METERS(h * 0.5f));

    b2FixtureDef fd;
    fd.shape = &shape;
    fd.density = dynamic ? 1.0f : 0.0f;
    fd.friction = 0.4f;
    fd.restitution = 0.05f;

    body->CreateFixture(&fd);
    return body;
}

b2Body* Physics::CreateCircle(float x, float y, float radius, bool dynamic,
    float density, float friction, float restitution) {
    b2BodyDef bd;
    bd.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));
    bd.type = dynamic ? b2_dynamicBody : b2_staticBody;
    b2Body* body = world->CreateBody(&bd);

    b2CircleShape shape;
    shape.m_p.Set(0.0f, 0.0f);
    shape.m_radius = PIXEL_TO_METERS(radius);

    b2FixtureDef fd;
    fd.shape = &shape;
    fd.density = dynamic ? density : 0.0f;
    fd.friction = friction;
    fd.restitution = restitution;

    body->CreateFixture(&fd);
    return body;
}

b2RevoluteJoint* Physics::CreateRevoluteJoint(
    b2Body* bodyA, b2Body* bodyB,
    float anchorX_px, float anchorY_px,
    float lowerDeg, float upperDeg,
    bool enableMotor, float motorSpeedRad, float maxMotorTorque
) {
    b2RevoluteJointDef jd;
    jd.Initialize(bodyA, bodyB, b2Vec2(PIXEL_TO_METERS(anchorX_px), PIXEL_TO_METERS(anchorY_px)));
    jd.enableLimit = true;
    jd.lowerAngle = lowerDeg * b2_pi / 180.0f;
    jd.upperAngle = upperDeg * b2_pi / 180.0f;
    jd.enableMotor = enableMotor;
    jd.motorSpeed = motorSpeedRad;
    jd.maxMotorTorque = maxMotorTorque;
    return (b2RevoluteJoint*)world->CreateJoint(&jd);
}

// Versión única y correcta de CreatePolygon
b2Body* Physics::CreatePolygon(const b2Vec2* vertices, int count, float friction, float restitution) {
    b2BodyDef bd;
    bd.type = b2_staticBody;
    b2Body* body = world->CreateBody(&bd);

    b2Vec2* meters_vertices = new b2Vec2[count];
    for (int i = 0; i < count; ++i) {
        meters_vertices[i].Set(PIXEL_TO_METERS(vertices[i].x), PIXEL_TO_METERS(vertices[i].y));
    }

    b2PolygonShape shape;
    shape.Set(meters_vertices, count);
    delete[] meters_vertices;

    b2FixtureDef fd;
    fd.shape = &shape;
    fd.density = 0.0f;
    fd.friction = friction;
    fd.restitution = restitution;
    body->CreateFixture(&fd);
    return body;
}

// <<< AÑADIDO: Implementación de CreateChain al final del fichero
b2Body* Physics::CreateChain(const b2Vec2* vertices, int count, float friction, float restitution) {
    b2BodyDef bd;
    bd.type = b2_staticBody;
    b2Body* body = world->CreateBody(&bd);

    // Convertir píxeles a metros
    b2Vec2* meters_vertices = new b2Vec2[count];
    for (int i = 0; i < count; ++i) {
        meters_vertices[i].Set(PIXEL_TO_METERS(vertices[i].x), PIXEL_TO_METERS(vertices[i].y));
    }

    // Crear un "Loop" (cadena cerrada) en lugar de un polígono
    b2ChainShape shape;
    shape.CreateLoop(meters_vertices, count);
    delete[] meters_vertices; // liberar memoria

    b2FixtureDef fd;
    fd.shape = &shape;
    fd.density = 0.0f;
    fd.friction = friction;
    fd.restitution = restitution;
    body->CreateFixture(&fd);
    return body;
}
// <<< NO HAY NINGUNA LLAVE '}' EXTRA AQUÍ AL FINAL