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
			if (ImGui::Checkbox("ImGui Test Window", &bShowExample));

			if (ImGui::Checkbox("Application Window", &bShowApplication));
		
			ImGui::EndMenu();
		}

		// WINDOW
		if (ImGui::BeginMenu("About.."))
		{
			ImGui::Text("Last Hope Engine");
			ImGui::Separator();

			ImGui::Text("3D engine for educational purposes");
			ImGui::Text("Developed in CITM, Terrassa");
			ImGui::Separator();

			ImGui::Text("Samuel Navarro & Roger Homedes");
			ImGui::Separator();

			if (ImGui::BeginMenu("Libraries"))
			{
				if (ImGui::MenuItem("ImGui Version:", ImGui::GetVersion()))
				{
					App->RequestBrowser("https://github.com/ocornut/imgui");
				}

				if (ImGui::MenuItem("MathGeoLib Version:", "2.0"))
				{
					App->RequestBrowser("https://github.com/juj/MathGeoLib");
				}

				if (ImGui::MenuItem("SDL Version:"))
				{
					App->RequestBrowser("https://www.libsdl.org/");
				}

				ImGui::EndMenu();
			}
			ImGui::Separator();

			if (ImGui::MenuItem("License"))
			{
				//App->RequestBrowser("");
			}

			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}
	
	//Test ImGui Window
	if (bShowExample)
		ImGui::ShowTestWindow();

	//Application Window
	if (bShowApplication)
		ApplicationWindow(true);

	// Rendering
	ImGui::Render();
	
	if (bExit == true)
	{
		return UPDATE_STOP;
	}
	return UPDATE_CONTINUE;
}


void ModuleEditor::ApplicationWindow(bool wActive)
{
	float size_x = 370.f;

	ImGui::SetNextWindowPos(ImVec2(App->window->screen_surface->w - size_x, 18.f));

	ImGuiWindowFlags flag = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoFocusOnAppearing;
	ImGui::Begin("Configuration", 0, ImVec2(size_x, App->window->screen_surface->h - 560.f), 0.8f, flag);
	{
		App->DrawUI();
	}

	ImGui::End();
}
