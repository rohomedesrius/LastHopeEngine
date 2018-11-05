#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ImGui/imgui_impl_sdl_gl3.h"

#define MAX_KEYS 300

ModuleInput::ModuleInput(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	keyboard = new KEY_STATE[MAX_KEYS];
	memset(keyboard, KEY_IDLE, sizeof(KEY_STATE) * MAX_KEYS);
}

// Destructor
ModuleInput::~ModuleInput()
{
	delete[] keyboard;
}

// Called before render is available
bool ModuleInput::Init()
{
	LOG("Init SDL input event system");
	bool ret = true;
	SDL_Init(0);

	if(SDL_InitSubSystem(SDL_INIT_EVENTS) < 0)
	{
		LOG("SDL_EVENTS could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	return ret;
}

// Called every draw update
update_status ModuleInput::PreUpdate(float dt)
{
	SDL_PumpEvents();

	const Uint8* keys = SDL_GetKeyboardState(NULL);
	
	for(int i = 0; i < MAX_KEYS; ++i)
	{
		if(keys[i] == 1)
		{
			if(keyboard[i] == KEY_IDLE)
				keyboard[i] = KEY_DOWN;
			else
				keyboard[i] = KEY_REPEAT;
		}
		else
		{
			if(keyboard[i] == KEY_REPEAT || keyboard[i] == KEY_DOWN)
				keyboard[i] = KEY_UP;
			else
				keyboard[i] = KEY_IDLE;
		}
	}

	Uint32 buttons = SDL_GetMouseState(&mouse_x, &mouse_y);

	mouse_x /= SCREEN_SIZE;
	mouse_y /= SCREEN_SIZE;
	mouse_z = 0;

	for(int i = 0; i < 5; ++i)
	{
		if(buttons & SDL_BUTTON(i))
		{
			if(mouse_buttons[i] == KEY_IDLE)
				mouse_buttons[i] = KEY_DOWN;
			else
				mouse_buttons[i] = KEY_REPEAT;
		}
		else
		{
			if(mouse_buttons[i] == KEY_REPEAT || mouse_buttons[i] == KEY_DOWN)
				mouse_buttons[i] = KEY_UP;
			else
				mouse_buttons[i] = KEY_IDLE;
		}
	}

	mouse_x_motion = mouse_y_motion = 0;

	SDL_Event e;

	while(SDL_PollEvent(&e))
	{
		ImGui_ImplSdlGL3_ProcessEvent(&e);
		switch(e.type)
		{
			case SDL_MOUSEWHEEL:
			mouse_z = e.wheel.y;
			break;

			case SDL_MOUSEMOTION:
			mouse_x = e.motion.x / SCREEN_SIZE;
			mouse_y = e.motion.y / SCREEN_SIZE;

			mouse_x_motion = e.motion.xrel / SCREEN_SIZE;
			mouse_y_motion = e.motion.yrel / SCREEN_SIZE;
			break;

			case SDL_DROPFILE:
			LOG("Input: Dropped File detected!");
			file_path = e.drop.file;
			App->renderer3D->Dropped();
			//App->scene->HandleDrop();
			SDL_free(e.drop.file);
			break;

			case SDL_QUIT:
			LOG("Input(SDL): Requested to Exit");
			App->ExitRequest();
			break;

			case SDL_WINDOWEVENT:
			{
				if(e.window.event == SDL_WINDOWEVENT_RESIZED)
					App->renderer3D->OnResize(e.window.data1, e.window.data2);
			}
		}
	}

	if (keyboard[SDL_SCANCODE_ESCAPE] == KEY_UP)
	{
		LOG("Input(ESC): Requested to Exit");
		App->ExitRequest();
	}

	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleInput::CleanUp()
{
	LOG("Quitting SDL input event subsystem");
	SDL_QuitSubSystem(SDL_INIT_EVENTS);
	return true;
}

void ModuleInput::DrawUI()
{
	static char* file_name = "-";
	if (GetFileDropped() != nullptr)
	{
		file_name = (char*)GetFileDropped();
	}

	if (ImGui::CollapsingHeader("Input"))
	{
		if (ImGui::TreeNodeEx("Mouse", ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::BeginGroup();
			{
				ImGui::Text("Mouse position:");
				ImGui::Text("Mouse motion:");
				ImGui::Text("Mouse wheel:");
				ImGui::EndGroup();
			}
			ImGui::SameLine();
			ImGui::BeginGroup();
			{
				ImGui::Text("%i, %i", GetMouseX(), GetMouseY());
				ImGui::Text("%i, %i", GetMouseXMotion(), GetMouseYMotion());
				ImGui::Text("%i", GetMouseZ());
				ImGui::EndGroup();
			}
			ImGui::TreePop();
		}
		if (ImGui::TreeNodeEx("Files", ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::Text("Latest dropped file:");
			ImVec4 col = ImColor(1.0f, 0.78f, 0.58f, 1.0f);
			ImGui::PushStyleColor(ImGuiCol_Text, col);
			ImGui::TextWrapped("%s", file_name);
			ImGui::PopStyleColor();
			ImGui::TreePop();
		}
	}
}

const char * ModuleInput::GetFileDropped()
{
	if (file_path != NULL)
		return file_path;
	else return nullptr;
}
