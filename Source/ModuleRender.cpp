#include "Globals.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "ModuleGame.h"
#include <math.h>

ModuleRender::ModuleRender(Application* app, bool start_enabled) : Module(app, start_enabled)
{
    background = BLACK;
}

// Destructor
ModuleRender::~ModuleRender()
{}

// Called before render is available
bool ModuleRender::Init()
{
	LOG("Creating Renderer context");
	bool ret = true;

	return ret;
}

// PreUpdate: clear buffer
update_status ModuleRender::PreUpdate()
{
	return UPDATE_CONTINUE;
}

// Update: debug camera
update_status ModuleRender::Update()
{
    ClearBackground(background);

    // NOTE: This function setups render batching system for
    // maximum performance, all consecutive Draw() calls are
    // not processed until EndDrawing() is called
    BeginDrawing();

	return UPDATE_CONTINUE;
}

// PostUpdate present buffer to screen
update_status ModuleRender::PostUpdate()
{
    // Draw everything in our batch!
    DrawFPS(10, 10);

    DrawText("SCORE:", 270, 10, 18, YELLOW);
    DrawText(TextFormat("%d", *score), 350, 10, 18, YELLOW);

    DrawText("LAST:", 270,25, 18, YELLOW);
    DrawText(TextFormat("%d", *score), 350, 25, 18, YELLOW);

    DrawText("BEST:", 270, 40, 18, YELLOW);
    DrawText(TextFormat("%d", *score), 350, 40, 18, YELLOW);

    DrawText("LIVES:", 270, 80, 18, YELLOW);
    DrawText(TextFormat("%d", *score), 350, 80, 18, YELLOW);

    DrawText("DOWN: START SIMULATION",10 ,29 ,18 ,YELLOW );
    DrawText("LEFT: LEFT FLIPPER", 10, 45, 18, YELLOW);
    DrawText("RIGHT: RIGHT FLIPPER", 10, 61, 18, YELLOW);
    DrawText("R: RESTART", 10, 76, 18, YELLOW);

    EndDrawing();

	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleRender::CleanUp()
{
	return true;
}

void ModuleRender::SetBackgroundColor(Color color)
{
	background = color;
}

// Draw to screen
bool ModuleRender::Draw(Texture2D texture, int x, int y, const Rectangle* section, double angle, int pivot_x, int pivot_y) const
{
	bool ret = true;

	float scale = 1.0f;
    Vector2 position = { (float)x, (float)y };
    Rectangle rect = { 0.f, 0.f, (float)texture.width, (float)texture.height };

    if (section != NULL) rect = *section;

    position.x = (float)(x-pivot_x) * scale + camera.x;
    position.y = (float)(y-pivot_y) * scale + camera.y;

	rect.width *= scale;
	rect.height *= scale;

    DrawTextureRec(texture, rect, position, WHITE);

	return ret;
}

