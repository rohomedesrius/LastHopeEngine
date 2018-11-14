#ifndef __SCENE_H__
#define __SCENE_H__

#include "Module.h"

class myQuadTree;

enum GameState
{
	EDIT,
	RUNNING, 
	PAUSED, 
};

class ModuleScene : public Module
{
public:

	ModuleScene(Application* app, bool start_enabled = true);
	~ModuleScene();

	bool Start();

	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

	void Draw();

	void HandleDrop();
	void LoadFBX(char* path);

	void DrawGrid(int size);

	//SCENE MANAGEMENT
	void Play();
	void Pause();
	void Stop();

	void SaveScene(const char* name);
	void LoadScene(const char* name);

	void ClearScene();

public:

	GameObject* root;
	std::vector<GameObject*> scene_game_objects;

	myQuadTree* my_quad_tree;

private:

	GameState game_state = EDIT;
	bool game_pause = false;
	bool game_play = false;

	Importer importer;

private:
	void ManageQuadTree();
	int current_gos = 0;
};

#endif //__MODULE_H__