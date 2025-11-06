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


	return true;
}

update_status ModulePhysics::PreUpdate()
{
	world->Step(1.0f / 60.0f, 6, 2);

	return UPDATE_CONTINUE;
}



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

	if (IsKeyPressed(KEY_DOWN) && gameStarted == false) {
		world->SetGravity({ 0.0f, 9.8f });
		delete player;
		player = CreateCircle(initialPos[0], initialPos[1], 10);
		//player->ApplyLinearImpulseToCenter(b2Vec2(0.0f, -12.0f), true);
		gameStarted = true;
	}
	else if (IsKeyPressed(KEY_R) && gameStarted == true) {
		gameStarted = false;
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
}