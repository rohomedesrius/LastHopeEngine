#include "Globals.h"
#include "Application.h"
#include "ModuleScene.h"
#include "GameObject.h"
#include "CompTransform.h"

#include "glew/include/glew.h"

ModuleScene::ModuleScene(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	root = new GameObject("root", true, true, nullptr);

	root->AddComponent(new CompTransform(root));
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
	LOG("Cleaning Scene");
	root->RemoveChildren(true);

	return true;
}

void ModuleScene::Draw()
{
	DrawGrid(10);
}

void ModuleScene::HandleDrop()
{
	FileExtensions extension = importer.GetExtension(App->input->GetFileDropped());

	switch (extension)
	{
	case FileExtensions::Scene3D:
	{
		LOG("Importing 3D Scene...");
		LoadFBX((char*)App->input->GetFileDropped());
		break;
	}
	case FileExtensions::Image:
	{
		LOG("Error! Can't import raw Images at the moment.");
		break;
	}
	case FileExtensions::Unsupported:
	{
		LOG("Error: File Type not supported");
		break;
	}
	}
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

void ModuleScene::LoadFBX(char* path)
{
	//GameObject* fbx = importer.ImportFBX(path);

	//fbx->SetParent(root);
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