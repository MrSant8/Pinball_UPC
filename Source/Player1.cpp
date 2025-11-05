#include "Globals.h"
#include "Application.h"
#include "Player1.h"

ModulePlayer::ModulePlayer(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading player");
	return true;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");

	return true;
}

// Update: draw background
update_status ModulePlayer::Update()
{
	if (IsKeyPressed(KEY_DOWN) && gameStarted == false) {
		ball->ApplyLinearImpulseToCenter(b2Vec2(0.0f, -12.0f), true);
		gameStarted = true;
	}
	else if (IsKeyPressed(KEY_R) && gameStarted == true) {
		gameStarted = false;
		//Destruir la bola i crear una nova a la posicio inicial
	}
	return UPDATE_CONTINUE;
}



