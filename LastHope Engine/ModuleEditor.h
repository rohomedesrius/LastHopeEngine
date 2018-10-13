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

	bool show_ui = false;

	bool show_properties = false;
	bool show_example = false;
	bool show_application = false;
	bool show_random = false;
	bool show_console = false;

public:

	void Draw();

	void RegisterLog(const char* log);

	void ManageUI();

	void LoadEdiConfig();

private:
	void ShowEngineConsole(bool* show);
	void ApplicationWindow();
	void PropertiesWindow();
	void RandomWindow();
};