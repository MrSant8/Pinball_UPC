#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModulePhysics.h"

#include "p2Point.h"

#include <math.h>
//Dona error a l'incloure units.h, de manera que ho fem aixi

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
	
	world = new b2World(b2Vec2 (0.0f,9.8f));


	


	return true;
}

update_status ModulePhysics::PreUpdate()
{

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

	

	
	return UPDATE_CONTINUE;
}






// Called before quitting
bool ModulePhysics::CleanUp()
{
	LOG("Destroying physics world");

	// Delete the whole physics world!
	

	return true;
}








