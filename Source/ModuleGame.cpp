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
