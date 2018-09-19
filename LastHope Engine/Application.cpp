#include "Application.h"

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
	//physics3D = new ModulePhysics3D(this);
	renderer3D = new ModuleRenderer3D(this);
	camera = new ModuleCamera3D(this);

	// The order of calls is very important!
	// Modules will Init() Start() and Update in this order
	// They will CleanUp() in reverse order

	// Main Modules
	AddModule(window);
	//AddModule(physics3D);
	AddModule(renderer3D);
	AddModule(camera);
	AddModule(input);
	//AddModule(audio);
	
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

	// Call Init() in all modules
	for (std::vector<Module*>::iterator item = list_modules.begin(); ret == true && item != list_modules.end(); item++)
	{
		(*item)->Init();
	}

	// After all Init calls we call Start() in all modules
	LOG("Application Start --------------");
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
	return ret;
}

bool Application::CleanUp()
{
	bool ret = true;

	for (std::vector<Module*>::reverse_iterator item = list_modules.rbegin(); ret == true && item != list_modules.rend(); item++)
	{
		ret = (*item)->CleanUp();
		delete (*item);
		*item = NULL;
	}
	return ret;
}

void Application::AddModule(Module* mod)
{
	list_modules.push_back(mod);
}