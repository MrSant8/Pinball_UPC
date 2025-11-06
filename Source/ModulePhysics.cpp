#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModulePhysics.h"

#include "p2Point.h"

#include <math.h>
//Dona error a l'incloure units.h, de manera que ho fem aixi

ModulePhysics::ModulePhysics(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	/*b2Vec2 gravity(0.0f, 9.8f);
	world = new b2World(gravity);*/
	world = NULL;
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
	
	world = new b2World(b2Vec2 (0.0f,0.0f));

	player = CreateCircle(initialPos[0], initialPos[1], 10);
	flipperD = CreateRectangle(215, 730, 50, 15);
	flipperE = CreateRectangle(400 ,400, 10, 10);

	crearMapa();
	return true;
}

update_status ModulePhysics::PreUpdate()
{
	world->Step(1.0f / 60.0f, 6, 2);

	return UPDATE_CONTINUE;
}



update_status ModulePhysics::PostUpdate()
{
	//FLIPPERS

	flipperAnchor = physics->CreateBox(1, 1, 1, 1, false);

	if (IsKeyPressed(KEY_F1))
	{
		debug = !debug;
	}

	if (!debug)
	{
		return UPDATE_CONTINUE;
	}

	if (IsKeyPressed(KEY_DOWN) && gameStarted == false) {
		world->SetGravity({ 0.0f, 9.8f });
		world->DestroyBody(player->body);
		delete player;
		player = CreateCircle(initialPos[0], initialPos[1], 10);
		player->body->ApplyLinearImpulseToCenter(b2Vec2(0.0f, -2.5f), true);
		gameStarted = true;
	}
	else if (IsKeyPressed(KEY_R) && gameStarted == true) {
		gameStarted = false;
		world->DestroyBody(player->body);
		delete player;
		player = CreateCircle(initialPos[0], initialPos[1], 10);
		world->SetGravity({ 0.0f, 0.0f });
	}

	for (b2Body* b = world->GetBodyList(); b; b = b->GetNext())
	{
		for (b2Fixture* f = b->GetFixtureList(); f; f = f->GetNext())
		{
			switch (f->GetType())
			{
				// Draw circles ------------------------------------------------
			case b2Shape::e_circle:
			{
				b2CircleShape* shape = (b2CircleShape*)f->GetShape();
				b2Vec2 pos = f->GetBody()->GetPosition();

				DrawCircle(METERS_TO_PIXELS(pos.x), METERS_TO_PIXELS(pos.y), (float)METERS_TO_PIXELS(shape->m_radius), WHITE);
			}
			break;
			case b2Shape::e_chain:
			{
				b2ChainShape* shape = (b2ChainShape*)f->GetShape();
				b2Vec2 prev, v;

				for (int32 i = 0; i < shape->m_count; ++i)
				{
					v = b->GetWorldPoint(shape->m_vertices[i]);
					if (i > 0)
						DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), GREEN);
					prev = v;
				}

				v = b->GetWorldPoint(shape->m_vertices[0]);
				DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), GREEN);
			}
			break;
			case b2Shape::e_polygon:
			{
				b2PolygonShape* polygonShape = (b2PolygonShape*)f->GetShape();
				int32 count = polygonShape->m_count;
				b2Vec2 prev, v;

				for (int32 i = 0; i < count; ++i)
				{
					v = b->GetWorldPoint(polygonShape->m_vertices[i]);
					if (i > 0)
						DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), RED);

					prev = v;
				}

				v = b->GetWorldPoint(polygonShape->m_vertices[0]);
				DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), RED);
			}
			break;
			}
		}
	}
	
	return UPDATE_CONTINUE;
}


PhysBody* ModulePhysics::CreateCircle(int x, int y, int radius)
{
	b2BodyDef body;
	body.type = b2_dynamicBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);

	b2CircleShape shape;
	shape.m_radius = PIXEL_TO_METERS(radius);
	b2FixtureDef fixture;
	fixture.shape = &shape;
	fixture.density = 1.0f;

	b->CreateFixture(&fixture);

	PhysBody* pbody = new PhysBody();
	pbody->body = b;

	return pbody;
}

PhysBody* ModulePhysics::CreateRectangle(int x, int y, int width, int height)
{
	PhysBody* pbody = new PhysBody();

	b2BodyDef body;
	body.type = b2_dynamicBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));
	body.userData.pointer = reinterpret_cast<uintptr_t>(pbody);

	b2Body* b = world->CreateBody(&body);
	b2PolygonShape box;
	box.SetAsBox(PIXEL_TO_METERS(width) * 0.5f, PIXEL_TO_METERS(height) * 0.5f);

	b2FixtureDef fixture;
	fixture.shape = &box;
	fixture.density = 1.0f;

	b->CreateFixture(&fixture);

	pbody->body = b;
	pbody->width = (int)(width * 0.5f);
	pbody->height = (int)(height * 0.5f);

	return pbody;
};

PhysBody* ModulePhysics::CreateChain(int x, int y, const int* points, int size)
{
	PhysBody* pbody = new PhysBody();

	b2BodyDef body;
	body.type = b2_dynamicBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));
	body.userData.pointer = reinterpret_cast<uintptr_t>(pbody);

	b2Body* b = world->CreateBody(&body);

	b2ChainShape shape;
	b2Vec2* p = new b2Vec2[size / 2];

	for (int i = 0; i < size / 2; ++i)
	{
		p[i].x = PIXEL_TO_METERS(points[i * 2 + 0]);
		p[i].y = PIXEL_TO_METERS(points[i * 2 + 1]);
	}

	shape.CreateLoop(p, size / 2);

	b2FixtureDef fixture;
	fixture.shape = &shape;

	b->CreateFixture(&fixture);

	delete p;

	pbody->body = b;
	pbody->width = pbody->height = 0;

	return pbody;
}

// Called before quitting
bool ModulePhysics::CleanUp()
{
	LOG("Destroying physics world");

	// Delete the whole physics world!
	

	return true;
}

void PhysBody::GetPosition(int& x, int& y) const
{
	b2Vec2 pos = body->GetPosition();
	x = METERS_TO_PIXELS(pos.x);
	y = METERS_TO_PIXELS(pos.y);
};

void ModulePhysics::crearMapa() {
	int mapaHD[102] = {
	360, 650,
	360, 170,
	346, 116,
	332, 88,
	296, 52,
	252, 28,
	217, 22,
	172, 22,
	138, 25,
	108, 37,
	70, 62,
	48, 84,
	34, 109,
	19, 143,
	17, 204,
	20, 227,
	28, 252,
	75, 382,
	71, 390,
	82, 397,
	82, 406,
	40, 442,
	23, 430,
	23, 591,
	31, 600,
	48, 601,
	54, 594,
	101, 618,
	101, 709,
	238, 710,
	237, 623,
	290, 593,
	297, 601,
	315, 601,
	321, 593,
	324, 462,
	293, 449,
	322, 358,
	320, 192,
	304, 178,
	287, 195,
	274, 191,
	241, 160,
	242, 119,
	266, 91,
	280, 91,
	303, 116,
	311, 139,
	319, 140,
	326, 163,
	330, 651
	};

	//Triangles raros que no son triangles
	int cosa1[14] = {
	73, 555,
	73, 487,
	74, 481,
	81, 482,
	116, 558,
	117, 565,
	110, 572
	};
	int cosa2[14] = {
	270, 555,
	270, 486,
	265, 481,
	261, 483,
	222, 560,
	222, 567,
	230, 574
	};
	int cosa3[16] = {
	65, 189,
	61, 185,
	61, 179,
	92, 89,
	99, 89,
	106, 99,
	106, 157,
	76, 189
	};
	//Crear limits mon
	PhysBody* Mapa = CreateChain(0, 105, mapaHD, 102);
	PhysBody* cosa_1 = CreateChain(0, 105, cosa1, 14);
	PhysBody* cosa_2 = CreateChain(0, 105, cosa2, 14);
	PhysBody* cosa_3 = CreateChain(0, 105, cosa3, 16);
}