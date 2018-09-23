#include "Globals.h"
#include "Application.h"
#include "ModuleEditor.h"

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_sdl_gl3.h"
#include "glew/include/glew.h"

ModuleEditor::ModuleEditor(Application* app, bool start_enabled) : Module(app, start_enabled)
{

}

ModuleEditor::~ModuleEditor()
{

}

// Load assets
bool ModuleEditor::Start()
{
	LOG("Loading Editor");
	bool ret = true;

	SDL_GLContext gl_context = SDL_GL_CreateContext(App->window->window);
	

	ImGui_ImplSdlGL3_Init(App->window->window);

	return ret;
}

update_status ModuleEditor::PreUpdate(float dt)
{
	ImGui_ImplSdlGL3_NewFrame(App->window->window);

	return UPDATE_CONTINUE;
}

bool ModuleEditor::CleanUp()
{
	LOG("Unloading Editor stuff");

	ImGui_ImplSdlGL3_Shutdown();

	return true;
}

// Update
update_status ModuleEditor::Update(float dt)
{
	if (ImGui::BeginMainMenuBar())
	{
		// FILE
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem(" Exit", "	Esc"))
			{
				bExit = true;
			}
			ImGui::EndMenu();
		}

		// WINDOW
		if (ImGui::BeginMenu("Window"))
		{
			if (ImGui::Checkbox("ImGui Test Window", &bShowExample))
			{
			}
			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}

	if (bShowExample)
	{
		ImGui::ShowTestWindow();
	}

	// Rendering
	ImGui::Render();
	
	if (bExit == true)
	{
		return UPDATE_STOP;
	}
	return UPDATE_CONTINUE;
}