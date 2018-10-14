#ifndef __MODULE_H__
#define __MODULE_H__

#include "Module.h"

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
};

#endif //__MODULE_H__