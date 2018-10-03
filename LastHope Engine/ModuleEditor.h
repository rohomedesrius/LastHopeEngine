#pragma once

#include "Module.h"
#include "Console.h"

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

	EngineConsole console;

	bool bShowExample = false;
	bool bShowApplication = true;
	bool bShowRandomWindow = false;
	bool bShowConsole = true;

public:

	void Draw();

	void RegisterLog(const char* log);

private:
	void ShowEngineConsole(bool* show);
	void ApplicationWindow(bool wActive);
};