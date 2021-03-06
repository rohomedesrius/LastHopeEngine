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

	void LoadConfig(JSONFile * file);
	void SaveConfig(JSONFile * file);

	SDL_Window* GetWindow() const;

	float GetAspectRatio();

public:
	//The window we'll be rendering to
	SDL_Window* window;

	//The surface contained by the window
	SDL_Surface* screen_surface;

private:

	int slider_width = NULL;
	int slider_height = NULL;

	bool fullscreen = false;
	bool fullscreen_desktop = false;
	bool maximize = false;
	bool borderless = false;
	bool resizable = false;

	int width = 1280;
	int height = 1024;

	float aspect_ratio = 0.0f;
};

#endif // __ModuleWindow_H__