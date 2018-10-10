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

	bool bShowExample;
	bool bShowApplication;
	bool bShowRandomWindow;
	bool bShowConsole;

public:

	void Draw();

	void RegisterLog(const char* log);

	void ManageUI();

	void LoadEdiConfig();

private:
	void ShowEngineConsole(bool* show);
	void ApplicationWindow(bool wActive);
	void RandomWindow(bool active);
};