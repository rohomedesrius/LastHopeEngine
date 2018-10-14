#include "Application.h"
#include "mmgr/mmgr.h"

#include "JSON.h"

Application::Application()
{
	frames = 0;
	last_frame_ms = -1;
	last_fps = -1;
	capped_ms = 1000 / 60;
	fps_counter = 0;

	window = new ModuleWindow(this);
	input = new ModuleInput(this);
	//audio = new ModuleAudio(this, true);
	editor = new ModuleEditor(this);
	renderer3D = new ModuleRenderer3D(this);
	camera = new ModuleCamera3D(this);
	scene = new ModuleScene(this);
	hardware = new ModuleHardware(this);
	filesys = new ModuleFileSystem(this);

	// The order of calls is very important!
	// Modules will Init() Start() and Update in this order
	// They will CleanUp() in reverse order

	// Main Modules
	AddModule(window);
	AddModule(editor);
	AddModule(scene);
	AddModule(camera);
	AddModule(input);
	AddModule(hardware);
	AddModule(filesys);
	//AddModule(audio);

	AddModule(renderer3D);
}

Application::~Application()
{
	std::vector<Module*>::iterator item = list_modules.end();

	while(item != list_modules.begin())
	{
		--item;
		delete (*item);
	}
}

bool Application::Init()
{
	bool ret = true;

	//Loading config file
	LoadConfig();

	// Call Init() in all modules
	for (std::vector<Module*>::iterator item = list_modules.begin(); ret == true && item != list_modules.end(); item++)
	{
		(*item)->Init();
	}

	// After all Init calls we call Start() in all modules
	LOG("\n-------------- Application Start --------------");
	for(std::vector<Module*>::iterator item = list_modules.begin(); ret == true && item != list_modules.end(); item++)
	{
		(*item)->Start();
	}

	return ret;
}


// ---------------------------------------------
void Application::PrepareUpdate()
{
	dt = (float)ms_timer.Read() / 1000.0f;
	ms_timer.Start();
}

// ---------------------------------------------
void Application::FinishUpdate()
{
	// Recap on framecount and fps
	++frames;
	++fps_counter;

	if(fps_timer.Read() >= 1000)
	{
		last_fps = fps_counter;
		fps_counter = 0;
		fps_timer.Start();
	}

	last_frame_ms = ms_timer.Read();

	int fps_cap_ms = (1000 / fps_cap);
	if (capped_ms != fps_cap_ms)
	{
		capped_ms = fps_cap_ms;
	}

	// cap fps
	if(last_frame_ms < capped_ms)
	{
		SDL_Delay(capped_ms - last_frame_ms);
	}

	//char t[50];
	//sprintf_s(t, "FPS: %d", (int)last_fps);
	//window->SetTitle(t);
}

// Call PreUpdate, Update and PostUpdate on all modules
update_status Application::Update()
{
	update_status ret = UPDATE_CONTINUE;
	PrepareUpdate();
	
	std::vector<Module*>::iterator item = list_modules.begin();
	
	for (; ret == UPDATE_CONTINUE && item != list_modules.end(); item++)
	{
		ret = (*item)->PreUpdate(dt);
	}

	for (item = list_modules.begin(); ret == UPDATE_CONTINUE && item != list_modules.end(); item++)
	{
		ret = (*item)->Update(dt);
	}

	for (item = list_modules.begin(); ret == UPDATE_CONTINUE && item != list_modules.end(); item++)
	{
		ret = (*item)->PostUpdate(dt);
	}

	FinishUpdate();

	if (exit)
		ret = UPDATE_STOP;

	return ret;
}

bool Application::CleanUp()
{
	bool ret = true;

	//JSON Save Config
	SaveConfig();

	delete(configJSON);

	LOG("Starting CleanUP: Closing Console");
	console_enabled = false;

	for (std::vector<Module*>::reverse_iterator item = list_modules.rbegin(); ret == true && item != list_modules.rend(); item++)
	{
		ret = (*item)->CleanUp();
		delete (*item);
		*item = NULL;
	}
	return ret;
}

void Application::DrawAllModules()
{
	std::vector<Module*>::iterator item = list_modules.begin();

	// Application Draw
	(this)->DrawUI();

	// Draw Modules UI
	for (item = list_modules.begin(); item != list_modules.end(); item++)
	{
		(*item)->DrawUI();
	}
}

void Application::DrawUI()
{
	// FPS
	if (!freeze_his)
	{
		for (int i = 0; i <= 99; i++)
		{
			if (i == 99)
			{
				msArr[i] = last_frame_ms;
				if (fps_counter == 0)
					fpsArr[i] = (float)last_fps;
			}
			else
			{
				msArr[i] = msArr[i + 1];
				if (fps_counter == 0)
					fpsArr[i] = fpsArr[i + 1];
			}
		}
	}

	if (ImGui::CollapsingHeader("Application", ImGuiTreeNodeFlags_DefaultOpen))
	{
		static char input[100];
		int size = sizeof(input) / sizeof(char);
		sprintf_s(input, size, "%s", name.c_str());

		ImGui::PushItemWidth(200);
		if (ImGui::InputText("App Name", input, size))
		{
			name.assign(input);
			SetAppTitle(name.c_str());
		}

		sprintf_s(input, size, "%s", organization.c_str());

		if (ImGui::InputText("Organization", input, sizeof(input) / sizeof(char)))
		{
			organization.assign(input);
		}

		ImGui::SliderInt("Max FPS", &fps_cap, 20.0f, 144.0f, "%.1f");
		fps = (fps_cap > 0) ? 1000 / fps_cap : 0;

		char title[25];
		sprintf_s(title, IM_ARRAYSIZE(title), "Framerate %.1f", fpsArr[99]);
		ImGui::PlotHistogram("##framerate", fpsArr, ((int)(sizeof(fpsArr) / sizeof(*fpsArr))), 0, title, 0.0f, 150.0f, ImVec2(310, 100));
		sprintf_s(title, IM_ARRAYSIZE(title), "Milliseconds %.1f", msArr[99]);
		ImGui::PlotHistogram("##milliseconds", msArr, ((int)(sizeof(msArr) / sizeof(*msArr))), 0, title, 0.0f, 9.0f, ImVec2(310, 100));

		// Memory
		sMStats stats = m_getMemoryStatistics();

		if (!freeze_his)
		{
			for (int i = 0; i <= 99; i++)
			{
				if (i == 99)
					memory[i] = (float)stats.totalReportedMemory;
				else
					memory[i] = memory[i + 1];
			}
		}

		sprintf_s(title, IM_ARRAYSIZE(title), "Memory Usage %i", (int)memory[99]);
		ImGui::PlotHistogram("##memory", memory, sizeof(memory) / sizeof(float), 0, title, 0.0f, (float)stats.peakReportedMemory * 1.2f, ImVec2(310, 100));

		// Freeze Displays
		ImGui::Checkbox(" Freeze Histograms", &freeze_his);
	}
}

void Application::AddModule(Module* mod)
{
	list_modules.push_back(mod);
}

//--------------------------------------------------------------------------------

void Application::RequestBrowser(const char* link)
{
	ShellExecute(0, 0, link, 0, 0, SW_SHOW);
}

void Application::ExitRequest()
{
	exit = true;
}

void Application::LoadConfig()
{
	configJSON = json->LoadJSON("config.json");

	if (configJSON->Exists())
	{
		const char* title = configJSON->GetInfoString("app.title");
		const char* org = configJSON->GetInfoString("app.org");

		SetAppTitle(title);
		SetAppOrg(org);

		std::vector<Module*>::iterator item = list_modules.begin();

		//Next modules
		for (item = list_modules.begin(); item != list_modules.end(); item++)
		{
			(*item)->LoadConfig(configJSON);
		}
	}
}

void Application::SaveConfig()
{
	if (configJSON->Exists())
	{
		configJSON->SetInfoString("app.title", App->name.c_str());
		configJSON->SetInfoString("app.org", App->organization.c_str());

		std::vector<Module*>::iterator item = list_modules.begin();

		for (item = list_modules.begin(); item != list_modules.end(); item++)
		{
			(*item)->SaveConfig(configJSON);
		}

		configJSON->SaveInfo();
	}	
}

void Application::SetAppTitle(const char* title)
{
	if (title != nullptr)
	{
		name = title;
		window->SetTitle(title);
	}
}

void Application::SetAppOrg(const char* org) 
{
	if (org != nullptr)
	{
		if (organization != org)
		{
			organization = org;
		}
	}
}
