#pragma once

#include "Module.h"

class GameObject;

class ModuleEditor : public Module
{
public:
	ModuleEditor(Application* app, bool start_enabled = true);
	~ModuleEditor();

	bool Start();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	bool CleanUp();

private:
	bool bExit = false;

	bool bShowExample = false;
	bool bShowApplication = false;
	bool bShowRandomWindow = false;

public:
	void ApplicationWindow(bool wActive);
};