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
	bool bShowApplication = true;
	bool bShowRandomWindow = false;
	bool bShowConsole = true;

private:
	static void ShowEngineConsole(bool* show);
	void ApplicationWindow(bool wActive);
};