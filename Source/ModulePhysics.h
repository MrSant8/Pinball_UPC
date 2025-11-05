#pragma once

#include "Module.h"
#include "Globals.h"

#include "box2d\box2d.h"


// Module --------------------------------------
class ModulePhysics : public Module, public b2ContactListener // TODO
{
public:
	ModulePhysics(Application* app, bool start_enabled = true);
	~ModulePhysics();

	bool Start();
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

	b2Body* CreatePolygon(const b2Vec2* vertices, int count, float friction = 0.3f, float restitution = 0.1f);

	b2Body* CreateChain(const b2Vec2* vertices, int count, float friction = 0.3f, float restitution = 0.1f);

	b2World* GetWorld() const { return world; }
	update_status PreUpdate();
	update_status PostUpdate();
	bool CleanUp();

	

private:
	b2World* world = nullptr;
	bool debug;
	
};