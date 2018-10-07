#include "Globals.h"
#include "Application.h"
#include "ModuleEditor.h"

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_sdl_gl3.h"
#include "glew/include/glew.h"

#include "PCG/pcg_basic.h"

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
				App->ExitRequest();
				LOG("Editor: Requested to Exit");
			}
			ImGui::EndMenu();
		}

		// WINDOW
		if (ImGui::BeginMenu("Window"))
		{
			if (ImGui::Checkbox("Engine Console", &bShowConsole));

			if (ImGui::Checkbox("ImGui Test Window", &bShowExample));

			if (ImGui::Checkbox("Random Generator", &bShowRandomWindow));

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
	
	// Engine Console
	if (bShowConsole)
		ShowEngineConsole(&bShowConsole);

	// Test ImGui Window
	if (bShowExample)
		ImGui::ShowTestWindow();

	// Random Generator Window
	if (bShowRandomWindow)
		RandomWindow(&bShowRandomWindow);

	// Application Window
	if (bShowApplication)
		ApplicationWindow(true);

	return UPDATE_CONTINUE;
}

void ModuleEditor::Draw()
{
	// Rendering
	ImGui::Render();
}

void ModuleEditor::RegisterLog(const char * log)
{
	console.AddLog(log);
}

void ModuleEditor::ShowEngineConsole(bool* show)
{
	ImGui::SetNextWindowPos(ImVec2(0.f, 500.f));
	console.Draw("LastHope Engine Console", show);
}

void ModuleEditor::ApplicationWindow(bool wActive)
{
	//ImGui::SetNextWindowPos(ImVec2(1000.f, 18.f));
	//ImGui::SetNextWindowSize(ImVec2(0.f, App->window->screen_surface->h - 18.f));

	ImGuiWindowFlags flag = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoFocusOnAppearing;
	ImGui::Begin("Configuration", 0, ImVec2(0.f, 0.f), 0.8f, flag);
	{
		App->DrawAllModules();
	}

	ImGui::End();
}

void ModuleEditor::RandomWindow(bool active)
{
	ImGui::Begin("Random Generator", 0, ImVec2(300.f, 125.f), 0.8f, ImGuiWindowFlags_NoResize);
	{

		if (ImGui::TreeNodeEx("Integer within Min & Max", ImGuiTreeNodeFlags_DefaultOpen))
		{
			static int max = 10;
			static int min = 0;
			static int result = 0;
			static char buffer[128];

			char title[40];
			sprintf_s(title, IM_ARRAYSIZE(title), "Min: %i", min);
			char title2[25];
			sprintf_s(title2, IM_ARRAYSIZE(title2), "Max: %i", max);
			ImGui::DragIntRange2("Range ##range", &min, &max, 1.f, 0.0f, 100000.0f, title, title2);
			
			if (ImGui::Button("Generate Result##RanInt"))
			{
				result = max - min;
				result = pcg32_boundedrand(result + 1);
				result += min;

				LOG("Random Generator: %i (Int from %i-%i)", result, min, max);
				sprintf_s(buffer, IM_ARRAYSIZE(buffer), "%i", result);
			}
			ImGui::SameLine();

			sprintf_s(title, IM_ARRAYSIZE(title), "within %i - %i ##Int", min, max);
			ImGui::PushItemWidth(40);
			ImGui::InputText(title, buffer, IM_ARRAYSIZE(buffer));

			ImGui::TreePop();
		}

		if (ImGui::TreeNodeEx("Float between 0 and 1", ImGuiTreeNodeFlags_DefaultOpen))
		{
			static float result = 0.f;
			static char buffer[128];

			if (ImGui::Button("Generate Result ##Float"))
			{
				int tmp = pcg32_boundedrand(10000);
				result = (float)tmp / 10000;

				LOG("Random Generator: %.4f (Float from 0-1)", result);
				sprintf_s(buffer, IM_ARRAYSIZE(buffer), "%.4f", result);
			}
			ImGui::SameLine();

			ImGui::PushItemWidth(60);
			ImGui::InputText("(%.4f) ##FloatResult", buffer, IM_ARRAYSIZE(buffer));


			ImGui::TreePop();
		}
	}

	ImGui::End();
}
