#include "Globals.h"
#include "Application.h"
#include "ModuleScene.h"

#include "glew/include/glew.h"

ModuleScene::ModuleScene(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	LOG("Creating Main Scene");
}

ModuleScene::~ModuleScene()
{

}

bool ModuleScene::Start()
{
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
	DrawGrid(7);
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
