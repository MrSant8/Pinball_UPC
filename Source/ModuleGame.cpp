#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleGame.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"

ModuleGame::ModuleGame(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	
}

ModuleGame::~ModuleGame()
{}

// Load assets
bool ModuleGame::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	mapa = LoadTexture("Assets/mapaHD.png");
	circle = LoadTexture("Assets/bola.png");


	//aqui s'ha de declarar totes les coordenades del mapa part1

	int mapaHD[44] = {
	328, 653,
	329, 164,
	317, 137,
	304, 114,
	282, 91,
	268, 90,
	243, 115,
	243, 163,
	275, 193,
	286, 193,
	298, 182,
	308, 182,
	322, 195,
	322, 362,
	292, 448,
	323, 464,
	324, 593,
	313, 601,
	299, 600,
	292, 595,
	240, 621,
	240, 653
	};

	App->physics->CreateChain(0, 105, mapaHD, 62); //CAL AJUSTAR COORDENADES SEGURAMENT


	//aqui fer lo mateix amb mapa part 2:

	return ret;
}

// Load assets
bool ModuleGame::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

// Update: draw background
update_status ModuleGame::Update()
{
	DrawTexture(mapa, 0, 105, WHITE);

	if (IsKeyPressed(KEY_ONE)) {

		bodies.push_back(App->physics->CreateCircle(GetMouseX(), GetMouseY(), 25));
	}

	for each (PhysBody * body in bodies)
	{
		int x, y;
		body->GetPosition(x, y);

		App->renderer->Draw(circle, x - circle.width * 0.5f, y - circle.height * 0.5f);
	}

	return UPDATE_CONTINUE;
}
