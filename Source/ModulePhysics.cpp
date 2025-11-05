#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModulePhysics.h"

#include "p2Point.h"

#include <math.h>
//Dona error a l'incloure units.h, de manera que ho fem aixi
#pragma region Units
#define PIXELS_PER_METER 50.0f          // 1 metro = 50 píxeles
#define METER_PER_PIXEL (1.0f / PIXELS_PER_METER)

#define PIXEL_TO_METERS(p) ((p) * METER_PER_PIXEL)  // pasar de píxeles a metros
#define METERS_TO_PIXELS(m) ((m) * PIXELS_PER_METER) // pasar de metros a píxeles
#pragma endregion
ModulePhysics::ModulePhysics(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	b2Vec2 gravity(0.0f, 9.8f);
	world = new b2World(gravity);
	debug = true;
}

// Destructor
ModulePhysics::~ModulePhysics()
{
	delete world;
	world = nullptr;
}

bool ModulePhysics::Start()
{
	LOG("Creating Physics 2D environment");
	
	return true;
}

update_status ModulePhysics::PreUpdate()
{

	return UPDATE_CONTINUE;
}

// 
update_status ModulePhysics::PostUpdate()
{
	

	if (IsKeyPressed(KEY_F1))
	{
		debug = !debug;
	}

	if (!debug)
	{
		return UPDATE_CONTINUE;
	}

	// Bonus code: this will iterate all objects in the world and draw the circles
	// You need to provide your own macro to translate meters to pixels
	/*for (b2Body* b = world->GetBodyList(); b; b = b->GetNext())
	{
		for(b2Fixture* f = b->GetFixtureList(); f; f = f->GetNext())
		{
			switch(f->GetType())
			{
				// Draw circles ------------------------------------------------
				case b2Shape::e_circle:
				{
					b2CircleShape* shape = (b2CircleShape*)f->GetShape();
					b2Vec2 pos = f->GetBody()->GetPosition();
					
					DrawCircle(METERS_TO_PIXELS(pos.x), METERS_TO_PIXELS(pos.y), (float)METERS_TO_PIXELS(shape->m_radius), Color{0, 0, 0, 128});
				}
				break;

				// Draw polygons ------------------------------------------------
				case b2Shape::e_polygon:
				{
					b2PolygonShape* polygonShape = (b2PolygonShape*)f->GetShape();
					int32 count = polygonShape->m_count;
					b2Vec2 prev, v;

					for(int32 i = 0; i < count; ++i)
					{
						v = b->GetWorldPoint(polygonShape->m_vertices[i]);
						if(i > 0)
							DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), RED);

						prev = v;
					}

					v = b->GetWorldPoint(polygonShape->m_vertices[0]);
					DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), RED);
				}
				break;

				// Draw chains contour -------------------------------------------
				case b2Shape::e_chain:
				{
					b2ChainShape* shape = (b2ChainShape*)f->GetShape();
					b2Vec2 prev, v;

					for(int32 i = 0; i < shape->m_count; ++i)
					{
						v = b->GetWorldPoint(shape->m_vertices[i]);
						if(i > 0)
							DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), GREEN);
						prev = v;
					}

					v = b->GetWorldPoint(shape->m_vertices[0]);
					DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), GREEN);
				}
				break;

				// Draw a single segment(edge) ----------------------------------
				case b2Shape::e_edge:
				{
					b2EdgeShape* shape = (b2EdgeShape*)f->GetShape();
					b2Vec2 v1, v2;

					v1 = b->GetWorldPoint(shape->m_vertex0);
					v1 = b->GetWorldPoint(shape->m_vertex1);
					DrawLine(METERS_TO_PIXELS(v1.x), METERS_TO_PIXELS(v1.y), METERS_TO_PIXELS(v2.x), METERS_TO_PIXELS(v2.y), BLUE);
				}
				break;
			}

			
		}
	}//*/

	
	return UPDATE_CONTINUE;
}


// Called before quitting
bool ModulePhysics::CleanUp()
{
	LOG("Destroying physics world");

	// Delete the whole physics world!
	

	return true;
}

b2Body* ModulePhysics::CreateBox(float x, float y, float w, float h, bool dynamic) {
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

b2Body* ModulePhysics::CreateCircle(float x, float y, float radius, bool dynamic,
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

b2RevoluteJoint* ModulePhysics::CreateRevoluteJoint(
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
b2Body* ModulePhysics::CreatePolygon(const b2Vec2* vertices, int count, float friction, float restitution) {
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

b2Body* ModulePhysics::CreateChain(const b2Vec2* vertices, int count, float friction, float restitution) {
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