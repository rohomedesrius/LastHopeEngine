#ifndef __ModuleWindow_H__
#define __ModuleWindow_H__

#include "Module.h"
#include "SDL/include/SDL.h"

class Application;

class ModuleWindow : public Module
{
public:

	ModuleWindow(Application* app, bool start_enabled = true);

	// Destructor
	virtual ~ModuleWindow();

	bool Init();
	bool CleanUp();

	void DrawUI();

	void SetTitle(const char* title);

	void LoadWinConfig();

public:
	//The window we'll be rendering to
	SDL_Window* window;

	//The surface contained by the window
	SDL_Surface* screen_surface;

private:

	bool fullscreen;
	bool fullscreen_desktop;
	bool maximize;
	bool borderless;
	bool resizable;

	int width = 0;
	int height = 0;
};

#endif // __ModuleWindow_H__