#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleGame.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"

//Quan creem la bola aqui hem de fer que Player1->ball = la bola que hem creat












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
		//new Circle(App->physics, 20, 20, this,circle);
		/*entities.emplace_back(new Circle(App->physics, GetMouseX(), GetMouseY(), this, circle));

		App->renderer->Draw(circle, 40 - circle.width * 0.5f, 40 - circle.height * 0.5f);*/
	}

	return UPDATE_CONTINUE;
}
