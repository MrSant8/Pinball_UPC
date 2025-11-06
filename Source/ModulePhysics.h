#pragma once

#include "Module.h"
#include "Globals.h"

#include "box2d\box2d.h"

#pragma region Units
#define PIXELS_PER_METER 50.0f          // 1 metro = 50 p�xeles
#define METER_PER_PIXEL (1.0f / PIXELS_PER_METER)

#define PIXEL_TO_METERS(p) ((p) * METER_PER_PIXEL)  // pasar de p�xeles a metros
#define METERS_TO_PIXELS(m) ((m) * PIXELS_PER_METER) // pasar de metros a p�xeles
#pragma endregion



class PhysBody {
public:
	PhysBody() {};

	void GetPosition(int& x, int& y) const;
	int width,height;

public:
	b2Body* body;
	Module* listener;
};



// Module --------------------------------------
class ModulePhysics : public Module , public b2ContactListener
{
public:
	ModulePhysics(Application* app, bool start_enabled = true);
	~ModulePhysics();

	bool Start();
	update_status PreUpdate();
	update_status PostUpdate();
	bool CleanUp();

	PhysBody* CreateCircle(int x, int y, int radius);
	PhysBody* CreateRectangle(int x, int y, int width, int height);
	PhysBody* CreateChain(int x, int y, const int* points, int size);

	int initialPos[2] = {345, 700};
	PhysBody* player;
	PhysBody* flipperD;
	PhysBody* flipperE;

	PhysBody* bumper1;
	PhysBody* bumper2;
	PhysBody* bumper3;

	b2RevoluteJoint* jointD = nullptr;
	b2RevoluteJoint* jointE = nullptr;

	PhysBody* flipperAnchor = nullptr;
	PhysBody* CreateBox(float x, float y, float w, float h, bool dynamic);
	b2RevoluteJoint* CreateRevoluteJoint(PhysBody* bodyA, PhysBody* bodyB,
		float anchorX_px, float anchorY_px,
		float lowerDeg, float upperDeg,
		bool enableMotor, float motorSpeedRad, float maxMotorTorque
	);


	bool gameStarted = false;

	void crearMapa();
	void BeginContact(b2Contact* contact);
	void OnCollision(PhysBody* bodyA, PhysBody* bodyB);

	int livesRemaining = 3;
	int score = 0;
	int highScore = 0;
	int lastScore = 0;

private:

	bool debug;
	b2World* world;
	b2MouseJoint* mouse_joint;
	b2Body* ground;

};