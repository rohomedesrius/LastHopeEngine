#pragma once

#include "Module.h"
#include "Console.h"

class GameObject;

struct Style
{
	bool active = false;

	// c stands for Color
	ImVec4 c_text = {};
	ImVec4 c_head = {};
	ImVec4 c_area = {};
	ImVec4 c_body = {};
	ImVec4 c_pop = {};
};

class ModuleEditor : public Module
{
public:
	ModuleEditor(Application* app, bool start_enabled = true);
	~ModuleEditor();

	bool Start();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	bool CleanUp();

	void DrawUI();

private:

	EngineConsole console;

	Style style_classic;
	Style style_blue;
	Style style_forest;
	Style style_dark;

	bool show_ui = true;

	bool show_properties = false;
	bool show_example = false;
	bool show_application = false;
	bool show_random = false;
	bool show_console = false;

	void SetStyle(Style style);
	void InitStyles();

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