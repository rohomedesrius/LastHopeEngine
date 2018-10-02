#pragma once
#include "Module.h"
#include "Globals.h"

class Plane_P;

class ModuleScene : public Module
{
public:
	ModuleScene(Application* app, bool start_enabled = true);
	~ModuleScene();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();


public:

	Plane_P* grid;
};
