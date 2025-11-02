#include "Scene.h"
#include "Units.h"

Scene::Scene(Physics* p) : physics(p) {
    // Carga la imagen desde la ruta especificada.
    backgroundTexture = LoadTexture("Assets/fondo.jpg");
}
Scene::~Scene() {
    UnloadTexture(backgroundTexture);
}

void Scene::CreateInitialScene() {

    // 3. Bumpers (obstáculos circulares)
    bumpers[0] = physics->CreateCircle(400, 150, bumperRadius, false, 0, 0.1f, 1.5f);
    bumpers[1] = physics->CreateCircle(250, 250, bumperRadius, false, 0, 0.1f, 1.5f);
    bumpers[2] = physics->CreateCircle(550, 250, bumperRadius, false, 0, 0.1f, 1.5f);

    // 4. Flippers
    b2Body* flipperAnchor = physics->CreateBox(400, 600, 1, 1, false);
    leftFlipper = physics->CreateBox(310, 550, 90, 15, true);
    rightFlipper = physics->CreateBox(490, 550, 90, 15, true);
    leftFlipperJoint = physics->CreateRevoluteJoint(flipperAnchor, leftFlipper, 260, 550, -25.0f, 20.0f, true, 15.0f, 120.0f);
    rightFlipperJoint = physics->CreateRevoluteJoint(flipperAnchor, rightFlipper, 540, 550, -20.0f, 25.0f, true, -15.0f, 120.0f);

    // 5. Bola
    ball = physics->CreateCircle(765, 580, ballRadius, true, 1.0f, 0.2f, 0.3f);
}

void Scene::Draw() {
    // 1. Dibuja el fondo.
    DrawTexture(backgroundTexture, 0, 0, WHITE);

    // 2. Dibuja todos los cuerpos ESTÁTICOS (paredes, bumpers, etc.).
    for (b2Body* b = physics->GetWorld()->GetBodyList(); b; b = b->GetNext()) {
        if (b->GetType() == b2_staticBody) {
            Color color = DARKBLUE;
            if (b == leftSlingshot || b == rightSlingshot) color = BLUE;

            if (b == bumpers[0] || b == bumpers[1] || b == bumpers[2]) {
                b2Vec2 p = b->GetPosition();
                DrawCircle((int)METERS_TO_PIXELS(p.x), (int)METERS_TO_PIXELS(p.y), bumperRadius, YELLOW);
            }
            else {
                DrawBody(b, color);
            }
        }
    }

    // 3. Dibuja los cuerpos DINÁMICOS al final para que queden por encima.
    DrawBody(leftFlipper, ORANGE);
    DrawBody(rightFlipper, ORANGE);

    // La bola es lo último que se dibuja para que siempre sea visible.
    if (ball) {
        b2Vec2 p = ball->GetPosition();
        DrawCircleGradient((int)METERS_TO_PIXELS(p.x), (int)METERS_TO_PIXELS(p.y), ballRadius, PINK, RED);
    }
}

void Scene::DrawBody(b2Body* body, Color color) {
    if (!body) return;
    for (b2Fixture* f = body->GetFixtureList(); f; f = f->GetNext()) {
        if (f->GetType() == b2Shape::e_polygon) {
            b2PolygonShape* shape = (b2PolygonShape*)f->GetShape();
            int vertexCount = shape->m_count;
            Vector2 vertices[b2_maxPolygonVertices];
            for (int i = 0; i < vertexCount; ++i) {
                b2Vec2 worldPos = body->GetWorldPoint(shape->m_vertices[i]);
                vertices[i] = { (float)METERS_TO_PIXELS(worldPos.x), (float)METERS_TO_PIXELS(worldPos.y) };
            }
            for (int i = 1; i < vertexCount - 1; ++i) {
                DrawTriangle(vertices[0], vertices[i], vertices[i + 1], color);
            }
        }
    }
}

void Scene::DrawDebug() {}

bool Scene::IsBallLost() const {
    if (ball == nullptr) return false;
    return METERS_TO_PIXELS(ball->GetPosition().y) > 620;
}

void Scene::ResetBall() {
    if (ball) {
        ball->SetTransform(b2Vec2(PIXEL_TO_METERS(765.0f), PIXEL_TO_METERS(580.0f)), 0);
        ball->SetLinearVelocity(b2Vec2_zero);
        ball->SetAngularVelocity(0);
    }
}