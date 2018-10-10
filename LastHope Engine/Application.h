#pragma once

#include "Globals.h"
#include "Timer.h"
#include "Module.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleAudio.h"
#include "ModuleEditor.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"
#include "ModuleScene.h"
#include "ModuleHardware.h"
#include "ModuleFileSystem.h"

#include "ImGui/imgui.h"

class JSON;
class JSONFile;

class Application
{
public:
	ModuleWindow* window;
	ModuleInput* input;
	//ModuleAudio* audio;
	ModuleEditor* editor;

	ModuleRenderer3D* renderer3D;
	ModuleCamera3D* camera;
	ModuleScene* scene;
	ModuleHardware* hardware;
	ModuleFileSystem* filesys;

	std::string name = "No name";
	std::string organization = "No org";

	JSON* json = nullptr;
	JSONFile* configJSON = nullptr;

private:

	Timer	ms_timer;

	// FPS
	Timer	fps_timer;
	Uint32	frames;
	float	dt;
	int		fps_counter;
	int		last_frame_ms;
	int		last_fps;
	int		capped_ms;
	std::vector<Module*> list_modules;

	// Config Temporal
	int fps_cap = 60.0f;

	float fpsArr[100] = {};
	float msArr[100] = {};
	float fps = 60.0f;
	bool bFreeze = false;

	// Memory
	float memory[100] = {};

public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();

	void DrawAllModules();
	void DrawUI();

	//Links request
	void RequestBrowser(const char* link);

	// Exit request
	void ExitRequest();

	// Console
	bool console_enabled = true;

	//App Info
	void SetAppTitle(const char* title);
	void SetAppOrg(const char* org);
	void SetAppVer(const char* version);

	//JSON
	void LoadAppConfig();

private:

	void AddModule(Module* mod);
	void PrepareUpdate();
	void FinishUpdate();

	bool exit = false;

};

extern Application* App;