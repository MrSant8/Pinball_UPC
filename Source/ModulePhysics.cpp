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
	world->SetContactListener(this);

	player = CreateCircle(initialPos[0], initialPos[1], 10);

	//FLIPPERS
	flipperD = CreateRectangle(212, 730, 50, 15, true);
	flipperE = CreateRectangle(127, 730, 50, 15, true);
	flipperAnchor = CreateBox(1, 1, 1, 1, false);


	float yFlipper = 605.0f; //num PETIT -> flippers DALT, num GRAN -> flippers BAIX
	float xLeft = 131.0f;    //num GRAN -> DRETA
	float xRight = 209.0f;   //num PETIT -> ESQUERRA

	flipperD->body->SetAngularDamping(5.0f);
	flipperE->body->SetAngularDamping(5.0f);

	float leftAnchorX = xLeft - flipperE->width * 0.5f + 6.0f;
	float rightAnchorX = xRight + flipperD->width * 0.5f - 6.0f;

	float lower = -30.0f, upper = 30.0f;

	jointE = CreateRevoluteJoint(flipperAnchor, flipperE, leftAnchorX, yFlipper, lower, upper, true, -8.0f, 120.0f);
	jointD = CreateRevoluteJoint(flipperAnchor, flipperD, rightAnchorX, yFlipper, lower, upper, true, 8.0f, 120.0f);

	bumper1 = CreateCircle(126, 400, 24);
	bumper1->body->SetType(b2_staticBody);
	bumper2 = CreateCircle(220, 400, 24);
	bumper2->body->SetType(b2_staticBody);
	bumper3 = CreateCircle(173, 324, 24);
	bumper3->body->SetType(b2_staticBody);

	crearMapa();
	return true;
}

update_status ModulePhysics::PreUpdate()
{
	world->Step(1.0f / 60.0f, 6, 2);
	player->listener->OnCollision(player, bumper1);
	player->listener->OnCollision(player, bumper2);
	player->listener->OnCollision(player, bumper3);
	return UPDATE_CONTINUE;
}



update_status ModulePhysics::PostUpdate()
{
	//FLIPPERS

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
	pbody->listener = this;
	return pbody;
}

PhysBody* ModulePhysics::CreateRectangle(int x, int y, int width, int height,bool dynamic)
{
	PhysBody* pbody = new PhysBody();

	b2BodyDef body;
	body.type = b2_kinematicBody;
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
	body.type = b2_staticBody;
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

PhysBody* ModulePhysics::CreateBox(float x, float y, float w, float h, bool dynamic) {
	PhysBody* pbody = new PhysBody();

	b2BodyDef bd;
	bd.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));
	bd.type = dynamic ? b2_dynamicBody : b2_kinematicBody;
	b2Body* b = world->CreateBody(&bd);

	b2PolygonShape shape;
	shape.SetAsBox(PIXEL_TO_METERS(w * 0.5f), PIXEL_TO_METERS(h * 0.5f));

	b2FixtureDef fd;
	fd.shape = &shape;
	fd.density = dynamic ? 1.0f : 0.0f;
	fd.friction = 0.4f;
	fd.restitution = 0.05f;

	b->CreateFixture(&fd);

	pbody->body = b;
	return pbody;
}
b2RevoluteJoint* ModulePhysics::CreateRevoluteJoint(
	PhysBody* bodyA, PhysBody* bodyB,
	float anchorX_px, float anchorY_px,
	float lowerDeg, float upperDeg,
	bool enableMotor, float motorSpeedRad, float maxMotorTorque) 
{
	b2RevoluteJointDef jd;
	jd.Initialize(bodyA->body, bodyB->body, b2Vec2(PIXEL_TO_METERS(anchorX_px), PIXEL_TO_METERS(anchorY_px)));
	jd.enableLimit = true;
	jd.lowerAngle = lowerDeg * b2_pi / 180.0f;
	jd.upperAngle = upperDeg * b2_pi / 180.0f;
	jd.enableMotor = enableMotor;
	jd.motorSpeed = motorSpeedRad;
	jd.maxMotorTorque = maxMotorTorque;
	return (b2RevoluteJoint*)world->CreateJoint(&jd);
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
	357, 650,
	358, 170,
	346, 116,
	332, 88,
	296, 52,
	251, 32,
	217, 24,
	171, 24,
	140, 28,
	108, 37,
	72, 64,
	50, 85,
	36, 109,
	21, 154,
	19, 201,
	23, 223,
	30, 251,
	79, 381,
	71, 390,
	82, 397,
	82, 406,
	40, 442,
	26, 433,
	26, 591,
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
	320, 469,
	289, 451,
	319, 362,
	320, 196,
	304, 182,
	289, 196,
	273, 196,
	241, 163,
	240, 115,
	267, 88,
	282, 88,
	307, 113,
	317, 136,
	323, 138,
	332, 162,
	332, 654
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

	//Pals
	int pal[16] = {
	132, 161,
	132, 119,
	136, 115,
	140, 115,
	144, 119,
	144, 161,
	141, 165,
	136, 165
	};
	int palDiagonal[22] = {
	84, 263,
	88, 263,
	92, 259,
	92, 254,
	77, 215,
	71, 215,
	68, 219,
	69, 226,
	75, 243,
	78, 250,
	80, 259
	};
	//Crear limits mon
	PhysBody* Mapa = CreateChain(0, 105, mapaHD, 102);
	PhysBody* cosa_1 = CreateChain(0, 105, cosa1, 14);
	PhysBody* cosa_2 = CreateChain(0, 105, cosa2, 14);
	PhysBody* cosa_3 = CreateChain(0, 105, cosa3, 16);
	PhysBody* pal1 = CreateChain(0, 105, pal, 16);
	PhysBody* pal2 = CreateChain(36, 105, pal, 16);
	PhysBody* pal3 = CreateChain(72, 105, pal, 16);
	PhysBody* diagonal = CreateChain(0, 105, palDiagonal, 22);
}

void ModulePhysics::BeginContact(b2Contact* contact)
{
	b2BodyUserData dataA = contact->GetFixtureA()->GetBody()->GetUserData();
	b2BodyUserData dataB = contact->GetFixtureB()->GetBody()->GetUserData();

	PhysBody* physA = (PhysBody*)dataA.pointer;
	PhysBody* physB = (PhysBody*)dataB.pointer;

	if (physA && physA->listener != NULL)
		physA->listener->OnCollision(physA, physB);

	if (physB && physB->listener != NULL)
		physB->listener->OnCollision(physB, physA);
}

void ModulePhysics::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
}