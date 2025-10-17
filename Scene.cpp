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
    // Techo para que la bola no se escape por arriba.
    physics->CreateBox(400, 5, 800, 10, false);

    // 1. Paredes, curvas y guías
    {
        b2Vec2 left_lower[] = { {55, 600}, {55, 300}, {100, 450}, {100, 500}, {250, 550}, {260, 550} };
        physics->CreatePolygon(left_lower, 6, 0.2f, 0.4f);
        b2Vec2 left_upper[] = { {55, 300}, {55, 170}, {75, 120}, {115, 75}, {170, 45}, {250, 25}, {350, 25}, {350, 60} };
        leftWall = physics->CreatePolygon(left_upper, 8, 0.2f, 0.4f);

        b2Vec2 right_lower[] = { {745, 600}, {745, 300}, {700, 450}, {700, 500}, {550, 550}, {540, 550} };
        physics->CreatePolygon(right_lower, 6, 0.2f, 0.4f);
        b2Vec2 right_upper[] = { {745, 300}, {745, 170}, {725, 120}, {685, 75}, {630, 45}, {550, 25}, {450, 25}, {450, 60} };
        rightWall = physics->CreatePolygon(right_upper, 8, 0.2f, 0.4f);

        // Guía curva para la salida de la bola.
        b2Vec2 launcher_guide[] = { {705, 150}, {680, 120}, {680, 150}, {705, 180} };
        physics->CreatePolygon(launcher_guide, 4, 0.2f, 0.8f);

        b2Vec2 laneVertices[] = { {705, 180}, {705, 600}, {725, 600}, {725, 150} };
        physics->CreatePolygon(laneVertices, 4);

        b2Vec2 leftGuide[] = { {55, 400}, {120, 480}, {120, 450}, {85, 400} };
        physics->CreatePolygon(leftGuide, 4);
        b2Vec2 rightGuide[] = { {745, 400}, {680, 480}, {680, 450}, {715, 400} };
        physics->CreatePolygon(rightGuide, 4);

        // Suelo para el carril de lanzamiento (evita el "Game Over" instantáneo).
        physics->CreateBox(765, 595, 40, 10, false);
    }

    // 2. Slingshots (los triángulos que rebotan)
    leftSlingshot = physics->CreatePolygon(new b2Vec2[3]{ {100, 450}, {100, 500}, {250, 550} }, 3, 0.2f, 1.2f);
    rightSlingshot = physics->CreatePolygon(new b2Vec2[3]{ {700, 450}, {700, 500}, {550, 550} }, 3, 0.2f, 1.2f);

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