#pragma once

#include "Globals.h"
#include "Module.h"

#include "p2Point.h"

#include "raylib.h"
#include <vector>
#include "box2d\box2d.h"

class PhysBody;
class PhysicEntity;


class ModuleGame : public Module
{
public:
	ModuleGame(Application* app, bool start_enabled = true);
	~ModuleGame();

	bool Start();
	update_status Update();
	bool CleanUp();

public:

	Texture2D mapa;
	Texture2D circle;

	std::vector<PhysicEntity*> entities;
	std::vector<PhysBody*> bodies;
};
