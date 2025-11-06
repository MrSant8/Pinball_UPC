#pragma once

#include "Module.h"
#include "Globals.h"

#include "box2d\box2d.h"

#pragma region Units
#define PIXELS_PER_METER 50.0f          // 1 metro = 50 píxeles
#define METER_PER_PIXEL (1.0f / PIXELS_PER_METER)

#define PIXEL_TO_METERS(p) ((p) * METER_PER_PIXEL)  // pasar de píxeles a metros
#define METERS_TO_PIXELS(m) ((m) * PIXELS_PER_METER) // pasar de metros a píxeles
#pragma endregion



class PhysBody {
public:
	PhysBody() {};

	void GetPosition(int& x, int& y) const;
	int width,height;

public:
	b2Body* body;
};



// Module --------------------------------------
class ModulePhysics : public Module
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
	b2RevoluteJoint* leftJoint = nullptr;
	b2RevoluteJoint* rightJoint = nullptr;
	b2Body* flipperAnchor = nullptr;
	bool gameStarted = false;

	void crearMapa();

private:

	bool debug;
	b2World* world;
	b2MouseJoint* mouse_joint;
	b2Body* ground;

};