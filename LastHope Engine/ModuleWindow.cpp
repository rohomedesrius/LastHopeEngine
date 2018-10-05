#include "Globals.h"
#include "Application.h"
#include "ModuleWindow.h"

ModuleWindow::ModuleWindow(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	window = NULL;
	screen_surface = NULL;
}

// Destructor
ModuleWindow::~ModuleWindow()
{
}

// Called before render is available
bool ModuleWindow::Init()
{
	LOG("Init SDL window & surface");
	bool ret = true;

	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		LOG("SDL_VIDEO could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{
		//Create window
		int width = SCREEN_WIDTH * SCREEN_SIZE;
		int height = SCREEN_HEIGHT * SCREEN_SIZE;
		Uint32 flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;

		//Use OpenGL 2.1
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

		if(WIN_FULLSCREEN == true)
		{
			flags |= SDL_WINDOW_FULLSCREEN;
		}

		if(WIN_RESIZABLE == true)
		{
			flags |= SDL_WINDOW_RESIZABLE;
		}

		if(WIN_BORDERLESS == true)
		{
			flags |= SDL_WINDOW_BORDERLESS;
		}

		if(WIN_FULLSCREEN_DESKTOP == true)
		{
			flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
		}

		window = SDL_CreateWindow(App->name.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, flags);

		if(window == NULL)
		{
			LOG("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			ret = false;
		}
		else
		{
			//Get window surface
			screen_surface = SDL_GetWindowSurface(window);
		}
	}

	return ret;
}

// Called before quitting
bool ModuleWindow::CleanUp()
{
	LOG("Destroying SDL window and quitting all SDL systems");

	//Destroy window
	if(window != NULL)
	{
		SDL_DestroyWindow(window);
	}

	//Quit SDL subsystems
	SDL_Quit();
	return true;
}

void ModuleWindow::DrawUI()
{
	if (ImGui::CollapsingHeader("Window"))
	{
		Uint32 flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;
		
		ImGui::Text("Current Size: %i x %i", screen_surface->w, screen_surface->h);

		if (!maximize && !fullscreen_desktop)
		{
			// WIDTH
			if (ImGui::SliderInt("##Width", &screen_surface->w, 600, 1920))
			{
				SDL_SetWindowSize(window, screen_surface->w, screen_surface->h);
				App->renderer3D->OnResize(screen_surface->w, screen_surface->h);
			}
			if (ImGui::IsItemHovered())
				ImGui::SetTooltip("Width");


			// HEIGHT
			if (ImGui::SliderInt("##Height", &screen_surface->h, 400, 1440))
			{
				SDL_SetWindowSize(window, screen_surface->w, screen_surface->h);
				App->renderer3D->OnResize(screen_surface->w, screen_surface->h);
			}
			if (ImGui::IsItemHovered())
				ImGui::SetTooltip("Height");
		}
		else
			ImGui::Text("Can't modify current Resolution!");

		ImGui::Separator();

		// CHECKBOX
		if (ImGui::Checkbox("FullScreen", &fullscreen))
		{
			if (fullscreen)
			{
				flags |= SDL_WINDOW_FULLSCREEN;
				SDL_SetWindowFullscreen(window, flags);
			}
			else
			{
				flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;
				SDL_SetWindowFullscreen(window, flags);
				fullscreen_desktop = maximize = false;
			}
		}

		ImGui::SameLine();
		if (ImGui::Checkbox("FullScreen Desktop", &fullscreen_desktop))
		{
			if (fullscreen_desktop) {
				flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
				SDL_SetWindowFullscreen(window, flags);
				fullscreen = true;
			}
			else
			{
				flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;
				SDL_SetWindowFullscreen(window, flags);
				fullscreen = maximize = false;
			}
		}

		if (ImGui::Checkbox("Borderless", &borderless))
		{
			if (borderless)
			{
				SDL_SetWindowBordered(window, SDL_FALSE);
			}
			else
			{
				SDL_SetWindowBordered(window, SDL_TRUE);
			}
		}

		ImGui::SameLine();
		if (ImGui::Checkbox("Maximize", &maximize))
		{
			if (maximize) {
				SDL_MaximizeWindow(window);
			}
			else
			{
				SDL_RestoreWindow(window);
			}
		}

		screen_surface = SDL_GetWindowSurface(window);
	}
}

void ModuleWindow::SetTitle(const char* title)
{
	SDL_SetWindowTitle(window, title);
}