#include "Globals.h"
#include "Application.h"
#include "ModuleScene.h"

#include "glew/include/glew.h"

ModuleScene::ModuleScene(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	
}

ModuleScene::~ModuleScene()
{

}

bool ModuleScene::Start()
{
	LOG("Creating Main Scene");

	// For Assigment 1
	App->renderer3D->LoadFBX("Assets\\BakerHouse.fbx");

	return false;
}

update_status ModuleScene::PreUpdate(float dt)

{
	return UPDATE_CONTINUE;
}

update_status ModuleScene::Update(float dt)
{
	return UPDATE_CONTINUE;
}

update_status ModuleScene::PostUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

bool ModuleScene::CleanUp()
{
	return true;
}

void ModuleScene::Draw()
{
	DrawGrid(10);
}

void ModuleScene::DrawGrid(int size)
{
	glBegin(GL_LINES);
	for (int i = -size; i <= size; i++)
	{
		glVertex3f(-size, 0, i);
		glVertex3f(size, 0, i);
	}
	for (int i = -size; i <= size; i++)
	{
		glVertex3f(i, 0, size);
		glVertex3f(i, 0, -size);
	}

	glEnd();
}

//Scene Management

void ModuleScene::Play()
{
	if (game_state != RUNNING)
	{
		game_state = RUNNING;
		game_pause = false;
		game_play = true;
	}
}

void ModuleScene::Pause()
{
	if (game_state == RUNNING && game_pause == false)
	{
		game_state = PAUSED;
		game_pause = true;
	}
}

void ModuleScene::Stop()
{
	if (game_state == RUNNING)
	{
		game_state = EDIT;
		game_pause = false;
		game_play = false;
	}
}