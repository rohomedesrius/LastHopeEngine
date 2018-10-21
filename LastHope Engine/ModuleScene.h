#ifndef __SCENE_H__
#define __SCENE_H__

#include "Module.h"

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

	void DrawGrid(int size);

	//SCENE MANAGEMENT
	void Play();
	void Pause();
	void Stop();

private:
	GameState game_state = EDIT;
	bool game_pause = false;
	bool game_play = false;
};

#endif //__MODULE_H__