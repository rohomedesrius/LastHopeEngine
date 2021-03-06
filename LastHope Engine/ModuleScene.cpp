#include "Globals.h"
#include "Application.h"
#include "ModuleScene.h"
#include "GameObject.h"
#include "CompTransform.h"

#include "glew/include/glew.h"

#include "JSON.h"

#include "QuadTree.h"

ModuleScene::ModuleScene(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	root = new GameObject("root", true, true, nullptr);

	root->AddComponent(new CompTransform(root));

	scene_game_objects.push_back(root);
}

ModuleScene::~ModuleScene()
{

}

bool ModuleScene::Start()
{
	LOG("Creating Main Scene");

	// For Assigment 1
	//App->renderer3D->LoadFBX("Assets\\BakerHouse.fbx");

	AABB box(vec(4, 2, 4), vec(-4, -2, -4));
	my_quad_tree = new myQuadTree(box);

	//QuadTree management
	current_gos = scene_game_objects.size();

	return false;
}

update_status ModuleScene::PreUpdate(float dt)
{
	if (current_gos != scene_game_objects.size())
	{
		ManageQuadTree();
		current_gos = scene_game_objects.size();
	}
	return UPDATE_CONTINUE;
}

update_status ModuleScene::Update(float dt)
{
	if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
	{
		GameObject* tmp = new GameObject("TEST", true, true, nullptr);
		scene_game_objects.push_back(tmp);
	}
	else if (App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN)
	{
		GameObject* tmp = new GameObject("TEST 2", true, true, root);
		scene_game_objects.push_back(tmp);
	}

	return UPDATE_CONTINUE;
}

update_status ModuleScene::PostUpdate(float dt)
{
	my_quad_tree->DebugDraw();

	return UPDATE_CONTINUE;
}

bool ModuleScene::CleanUp()
{
	LOG("Cleaning Scene");
	ClearScene();

	return true;
}

void ModuleScene::Draw()
{
	DrawGrid(10);

	// Render All GameObjects
	root->Draw();
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
		glVertex3f(-size, 0.f, i);
		glVertex3f(size, 0.f, i);
	}
	for (int i = -size; i <= size; i++)
	{
		glVertex3f(i, 0.f, size);
		glVertex3f(i, 0.f, -size);
	}

	glEnd();
}

void ModuleScene::LoadFBX(char* path)
{
	ClearScene();

	GameObject* fbx = importer.ImportFBX(path);

	fbx->SetAABB();

	//App->camera->FocusGeometry();

	fbx->SetParent(root);
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

void ModuleScene::SaveScene(const char* name)
{
	JSONFile new_scene;

	std::vector<GameObject*>::iterator it = scene_game_objects.begin();

	while (it != scene_game_objects.end())
	{
		(*it)->SaveGO(new_scene);
		it++;
	}

	new_scene.Create(name);
}

void ModuleScene::LoadScene(const char* name)
{

}

void ModuleScene::ClearScene()
{
	root->RemoveChildren(true);

	scene_game_objects.clear();
}

//QuadTreeManagement
void ModuleScene::ManageQuadTree()
{
	std::vector<GameObject*>::iterator it = scene_game_objects.begin();

	while (it != scene_game_objects.end())
	{
		if (!my_quad_tree->CheckGOs(*it))
		{
			my_quad_tree->AddGO(*it);
			it++;
		}
	}
}