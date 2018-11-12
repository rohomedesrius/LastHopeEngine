#include "Globals.h"
#include "Application.h"
#include "ModuleEditor.h"
#include "ModuleWindow.h"

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_sdl_gl3.h"
#include "glew/include/glew.h"

#include "PCG/pcg_basic.h"

#include "GameObject.h"

// Engine About Versions
#include "SDL/include/SDL_version.h"
#include "Assimp/include/version.h"
#include "PhysFS/include/physfs.h"

#include "JSON.h"

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

	SDL_GLContext gl_context = SDL_GL_CreateContext(App->window->GetWindow());
	
	InitStyles();
	HandleStyle();

	ImGui_ImplSdlGL3_Init(App->window->GetWindow());

	return ret;
}

update_status ModuleEditor::PreUpdate(float dt)
{
	ImGui_ImplSdlGL3_NewFrame(App->window->GetWindow());

	return UPDATE_CONTINUE;
}

bool ModuleEditor::CleanUp()
{
	LOG("Unloading Editor stuff");

	ImGui_ImplSdlGL3_Shutdown();

	return true;
}

void ModuleEditor::DrawUI()
{
	if (ImGui::CollapsingHeader("Editor"))
	{
		if (ImGui::Checkbox("ImGui's Classic Style", &style_classic.active))
		{
			ImGui::StyleColorsClassic();
			style_classic.active = true;
			style_blue.active = style_dark.active = style_forest.active = false;
			current_style = 0;
		}

		if (ImGui::Checkbox("Set Forest Style", &style_forest.active))
		{
			SetStyle(style_forest);
			style_forest.active = true;
			style_blue.active = style_dark.active = style_classic.active = false;
			current_style = 1;
		}

		if (ImGui::Checkbox("Set Blue Style", &style_blue.active))
		{
			SetStyle(style_blue);
			style_blue.active = true;
			style_forest.active = style_dark.active = style_classic.active = false;
			current_style = 2;
		}

		if (ImGui::Checkbox("Set Dark Style", &style_dark.active))
		{
			ImGui::StyleColorsDark();
			style_dark.active = true;
			style_forest.active = style_blue.active = style_classic.active = false;
			current_style = 3;
		}
	}
}

void ModuleEditor::LoadConfig(JSONFile * file)
{
	show_console = file->GetInfoBool("editor.console");
	show_application = file->GetInfoBool("editor.appli");
	show_random = file->GetInfoBool("editor.random");
	show_properties = file->GetInfoBool("editor.prop");
	show_hierachy = file->GetInfoBool("editor.hier");
	current_style = file->GetInfoNum("editor.style");
}

void ModuleEditor::SaveConfig(JSONFile * file)
{
	file->SetInfoBool("editor.console", show_console);
	file->SetInfoBool("editor.appli", show_application);
	file->SetInfoBool("editor.random", show_random);
	file->SetInfoBool("editor.prop", show_properties);
	file->SetInfoBool("editor.hier", show_hierachy);
	file->SetInfoNum("editor.style", current_style);
}

// Update
update_status ModuleEditor::Update(float dt)
{
	if (ImGui::BeginMainMenuBar())
	{
		// FILE
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem(" Save", "	Ctrl + S"))
			{
				//SAVE SCENE
				SaveLoadWindow(true);
				LOG("Editor: Scene Saved");
			}
			if (ImGui::MenuItem(" Load", "	Ctrl + L"))
			{
				//OPEN LOAD WINDOW
				SaveLoadWindow(false);
			}
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
			if (ImGui::Checkbox("Show UI", &show_ui));

			ImGui::Separator();

			if (ImGui::Checkbox("Application", &show_application));

			if (ImGui::Checkbox("Properties", &show_properties));

			if (ImGui::Checkbox("Console", &show_console));
			
			if (ImGui::Checkbox("Random Generator", &show_random));

			ImGui::EndMenu();
		}

		// WINDOW
		if (ImGui::BeginMenu("About.."))
		{
			if (ImGui::MenuItem("Last Hope Engine"))
			{
				App->RequestBrowser("https://github.com/rohomedesrius/LastHopeEngine");
			}

			ImGui::Separator();

			ImGui::Text("3D engine for educational purposes");
			ImGui::Text("Developed in CITM, Terrassa");
			ImGui::Separator();

			ImGui::Text("Samuel Navarro & Roger Homedes");
			ImGui::Separator();

			if (ImGui::BeginMenu("Libraries"))
			{
				char version[25] = "";
				if (ImGui::MenuItem("ImGui Version:", ImGui::GetVersion()))
				{
					App->RequestBrowser("https://github.com/ocornut/imgui");
				}

				if (ImGui::MenuItem("MathGeoLib Version:", "2.0"))
				{
					App->RequestBrowser("https://github.com/juj/MathGeoLib");
				}

				sprintf_s(version, "%i.%i.%i", SDL_MAJOR_VERSION, SDL_MINOR_VERSION, SDL_PATCHLEVEL);
				if (ImGui::MenuItem("SDL Version:", version))
				{
					App->RequestBrowser("https://www.libsdl.org/");
				}

				sprintf_s(version, "%s", glewGetString(GLEW_VERSION));
				if (ImGui::MenuItem("Glew Version:", version))
				{
					App->RequestBrowser("https://www.libsdl.org/");
				}

				sprintf_s(version, "%i.%i.%i", aiGetVersionMajor(), aiGetVersionMinor(), aiGetVersionRevision());
				if (ImGui::MenuItem("Assimp Version:", version))
				{
					App->RequestBrowser("https://github.com/assimp/assimp/releases/tag/v3.3/");
				}

				sprintf_s(version, "%i.%i.%i", PHYSFS_VER_MAJOR, PHYSFS_VER_MINOR, PHYSFS_VER_PATCH);
				if (ImGui::MenuItem("PhysFS Version:", version))
				{
					App->RequestBrowser("https://icculus.org/physfs/");
				}

				if (ImGui::MenuItem("DevIL Version:", "1.7.8"))
				{
					App->RequestBrowser("http://openil.sourceforge.net/");
				}

				if (ImGui::MenuItem("PCG Minimal C Version:", "0.9"))
				{
					App->RequestBrowser("http://www.pcg-random.org/download.html");
				}

				if (ImGui::MenuItem("Parson Version:", "-"))
				{
					App->RequestBrowser("https://github.com/kgabis/parson");
				}

				if (ImGui::MenuItem("MMGR Version:", "-"))
				{
					App->RequestBrowser("http://www.flipcode.com/archives/Presenting_A_Memory_Manager.shtml");
				}

				ImGui::EndMenu();
			}
			ImGui::Separator();

			if (ImGui::MenuItem("License"))
			{
				App->RequestBrowser("https://github.com/rohomedesrius/LastHopeEngine/blob/master/LICENSE");
			}

			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}

	if (show_ui)
	{
		ManageUI();
	}

	return UPDATE_CONTINUE;
}

void ModuleEditor::Draw() const
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
	if (show_application && show_properties)
	{
		ImGui::SetNextWindowPos(ImVec2(App->window->screen_surface->w / 4, (App->window->screen_surface->h / 4) * 3));
		ImGui::SetNextWindowSize(ImVec2((App->window->screen_surface->w / 4) * 2, App->window->screen_surface->h / 4));
	}
	else if (show_application && show_properties == false)
	{
		ImGui::SetNextWindowPos(ImVec2(0, (App->window->screen_surface->h / 4) * 3));
		ImGui::SetNextWindowSize(ImVec2((App->window->screen_surface->w / 4) * 3, App->window->screen_surface->h / 4));
	}
	else if (show_application == false && show_properties)
	{
		ImGui::SetNextWindowPos(ImVec2(App->window->screen_surface->w / 4, (App->window->screen_surface->h / 4) * 3));
		ImGui::SetNextWindowSize(ImVec2((App->window->screen_surface->w / 4) * 3, App->window->screen_surface->h / 4));
	}
	else if (show_application == false && show_properties == false)
	{
		ImGui::SetNextWindowPos(ImVec2(0, (App->window->screen_surface->h / 4) * 3));
		ImGui::SetNextWindowSize(ImVec2((App->window->screen_surface->w), App->window->screen_surface->h / 4));
	}

	console.Draw("LastHope Engine Console", show);
}

void ModuleEditor::ApplicationWindow()
{
	ImGuiWindowFlags flag = ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
	ImGui::Begin("Configuration", 0, ImVec2(0.f, 0.f), 0.8f, flag);
	{
		ImGui::SetWindowPos(ImVec2((App->window->screen_surface->w / 4)*3, 19));
		ImGui::SetWindowSize(ImVec2(App->window->screen_surface->w / 4, App->window->screen_surface->h - 19));
		
		App->DrawAllModules();
	}

	ImGui::End();
}

void ModuleEditor::PropertiesWindow()
{
	ImGuiWindowFlags flag = ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
	ImGui::Begin("Properties", 0, flag);
	{
		ImGui::SetWindowPos(ImVec2(0, App->window->screen_surface->h / 2));
		ImGui::SetWindowSize(ImVec2(App->window->screen_surface->w / 4, (App->window->screen_surface->h / 2) - 19));

		App->renderer3D->DrawProperties();
	}

	ImGui::End();
}

void ModuleEditor::HierachyWindow()
{
	ImGuiWindowFlags flag = ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
	ImGui::Begin("Hierachy", 0, flag);
	{
		ImGui::SetWindowPos(ImVec2(0, 19));
		ImGui::SetWindowSize(ImVec2(App->window->screen_surface->w / 4, App->window->screen_surface->h / 2));

		App->scene->root->DrawUI();
	}

	ImGui::End();
}

void ModuleEditor::RandomWindow()
{
	ImGui::Begin("Random Generator", 0, ImVec2(300.f, 125.f), 0.8f, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_AlwaysAutoResize);
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

				LOG("Random Generator: %i (%i-%i)", result, min, max);
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

				LOG("Random Generator: %.4f", result);
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

void ModuleEditor::SaveLoadWindow(bool is_saving)
{
	char input_name[256];

	if (is_saving)
	{
		ImGui::Begin("Save Scene");

		//if (ImGui::InputText("Scene Name:", input_name, IM_ARRAYSIZE(input_name), ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CallbackHistory, &TextEditCallbackStub, (void*)this))
		//{
		//	char* input_end = input_name + strlen(input_name);
		//	while (input_end > input_name && input_end[-1] == ' ') { input_end--; } *input_end = 0;
		//	if (input_name[0])
		//		App->scene->SaveScene(input_name);
		//	strcpy(input_name, "");
		//}
		ImGui::End();
	}
	else
	{
		ImGui::Begin("Load Scene");
		ImGui::End();
	}
}

void ModuleEditor::ManageUI()
{
	App->window->screen_surface = SDL_GetWindowSurface(App->window->GetWindow());

	// Application Window
	if (show_application)
		ApplicationWindow();

	// Properties Window
	if (show_properties)
		PropertiesWindow();

	// Hierachy Window
	if (show_hierachy)
		HierachyWindow();

	// Engine Console
	if (show_console)
		ShowEngineConsole(&show_console);

	// Random Generator Window
	if (show_random)
		RandomWindow();
}

void ModuleEditor::SetStyle(Style style)
{
	ImGuiStyle &current = ImGui::GetStyle();

	ImGui::StyleColorsLight();
	current.AntiAliasedLines = true;

	/*current.WindowPadding = ImVec2(10, 10);
	current.WindowRounding = 6;
	current.FramePadding = ImVec2(3, 3);
	current.FrameRounding = 2.6f;
	current.ItemSpacing = ImVec2(12, 4);
	current.ItemInnerSpacing = ImVec2(5, 5);*/

	current.ScrollbarSize = 24;
	current.ScrollbarRounding = 4;
	current.WindowRounding = 5.3f;

	current.Colors[ImGuiCol_Text] = ImVec4(style.c_text.x, style.c_text.y, style.c_text.z, 1.00f);
	current.Colors[ImGuiCol_TextDisabled] = ImVec4(style.c_text.x, style.c_text.y, style.c_text.z, 0.58f);
	current.Colors[ImGuiCol_TextSelectedBg] = ImVec4(style.c_text.x, style.c_text.y, style.c_text.z, 1.00f);
	current.Colors[ImGuiCol_WindowBg] = ImVec4(style.c_body.x, style.c_body.y, style.c_body.z, 0.95f);
	current.Colors[ImGuiCol_ChildWindowBg] = ImVec4(style.c_area.x, style.c_area.y, style.c_area.z, 0.58f);
	current.Colors[ImGuiCol_Border] = ImVec4(style.c_body.x, style.c_body.y, style.c_body.z, 0.00f);
	current.Colors[ImGuiCol_BorderShadow] = ImVec4(style.c_body.x, style.c_body.y, style.c_body.z, 0.00f);
	current.Colors[ImGuiCol_FrameBg] = ImVec4(style.c_area.x, style.c_area.y, style.c_area.z, 1.00f);
	current.Colors[ImGuiCol_FrameBgHovered] = ImVec4(style.c_head.x, style.c_head.y, style.c_head.z, 0.78f);
	current.Colors[ImGuiCol_FrameBgActive] = ImVec4(style.c_head.x, style.c_head.y, style.c_head.z, 1.00f);
	current.Colors[ImGuiCol_TitleBg] = ImVec4(style.c_area.x, style.c_area.y, style.c_area.z, 1.00f);
	current.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(style.c_area.x, style.c_area.y, style.c_area.z, 0.75f);
	current.Colors[ImGuiCol_TitleBgActive] = ImVec4(style.c_head.x, style.c_head.y, style.c_head.z, 1.00f);
	current.Colors[ImGuiCol_MenuBarBg] = ImVec4(style.c_area.x, style.c_area.y, style.c_area.z, 0.47f);
	current.Colors[ImGuiCol_ScrollbarBg] = ImVec4(style.c_area.x, style.c_area.y, style.c_area.z, 1.00f);
	current.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(style.c_head.x, style.c_head.y, style.c_head.z, 0.21f);
	current.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(style.c_head.x, style.c_head.y, style.c_head.z, 0.78f);
	current.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(style.c_head.x, style.c_head.y, style.c_head.z, 1.00f);
	current.Colors[ImGuiCol_CheckMark] = ImVec4(style.c_head.x, style.c_head.y, style.c_head.z, 0.95f);
	current.Colors[ImGuiCol_SliderGrab] = ImVec4(style.c_head.x, style.c_head.y, style.c_head.z, 0.50f);
	current.Colors[ImGuiCol_SliderGrabActive] = ImVec4(style.c_head.x, style.c_head.y, style.c_head.z, 1.00f);
	current.Colors[ImGuiCol_Button] = ImVec4(style.c_head.x, style.c_head.y, style.c_head.z, 0.50f);
	current.Colors[ImGuiCol_ButtonHovered] = ImVec4(style.c_head.x, style.c_head.y, style.c_head.z, 0.86f);
	current.Colors[ImGuiCol_ButtonActive] = ImVec4(style.c_head.x, style.c_head.y, style.c_head.z, 1.00f);
	current.Colors[ImGuiCol_Header] = ImVec4(style.c_head.x, style.c_head.y, style.c_head.z, 0.76f);
	current.Colors[ImGuiCol_HeaderHovered] = ImVec4(style.c_head.x, style.c_head.y, style.c_head.z, 0.86f);
	current.Colors[ImGuiCol_HeaderActive] = ImVec4(style.c_head.x, style.c_head.y, style.c_head.z, 1.00f);
	current.Colors[ImGuiCol_Column] = ImVec4(style.c_head.x, style.c_head.y, style.c_head.z, 0.32f);
	current.Colors[ImGuiCol_ColumnHovered] = ImVec4(style.c_head.x, style.c_head.y, style.c_head.z, 0.78f);
	current.Colors[ImGuiCol_ColumnActive] = ImVec4(style.c_head.x, style.c_head.y, style.c_head.z, 1.00f);
	current.Colors[ImGuiCol_ResizeGrip] = ImVec4(style.c_head.x, style.c_head.y, style.c_head.z, 0.15f);
	current.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(style.c_head.x, style.c_head.y, style.c_head.z, 0.78f);
	current.Colors[ImGuiCol_ResizeGripActive] = ImVec4(style.c_head.x, style.c_head.y, style.c_head.z, 1.00f);
	current.Colors[ImGuiCol_CloseButton] = ImVec4(style.c_text.x, style.c_text.y, style.c_text.z, 0.16f);
	current.Colors[ImGuiCol_CloseButtonHovered] = ImVec4(style.c_text.x, style.c_text.y, style.c_text.z, 0.39f);
	current.Colors[ImGuiCol_CloseButtonActive] = ImVec4(style.c_text.x, style.c_text.y, style.c_text.z, 1.00f);
	current.Colors[ImGuiCol_PlotLines] = ImVec4(style.c_text.x, style.c_text.y, style.c_text.z, 0.63f);
	current.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(style.c_head.x, style.c_head.y, style.c_head.z, 1.00f);
	current.Colors[ImGuiCol_PlotHistogram] = ImVec4(style.c_text.x, style.c_text.y, style.c_text.z, 0.63f);
	current.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(style.c_head.x, style.c_head.y, style.c_head.z, 1.00f);
	current.Colors[ImGuiCol_TextSelectedBg] = ImVec4(style.c_head.x, style.c_head.y, style.c_head.z, 0.43f);
	current.Colors[ImGuiCol_PopupBg] = ImVec4(style.c_pop.x, style.c_pop.y, style.c_pop.y, 0.92f);
	current.Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(style.c_area.x, style.c_area.y, style.c_area.z, 0.73f);
}

void ModuleEditor::InitStyles()
{
	style_blue.c_text = { 236 / 255.f, 240 / 255.f, 241 / 255.f, 0.f };
	style_blue.c_head = { 41 / 255.f, 128 / 255.f, 185 / 255.f, 0.f };
	style_blue.c_area = { 57 / 255.f,  79 / 255.f, 105 / 255.f, 0.f };
	style_blue.c_body = { 44 / 255.f,  62 / 255.f,  80 / 255.f, 0.f };
	style_blue.c_pop = { 33 / 255.f,  46 / 255.f,  60 / 255.f, 0.f };

	style_forest.c_text = { 211 / 255.f, 211 / 255.f, 211 / 255.f, 0.f };
	style_forest.c_head = { 95 / 255.f, 142 / 255.f,  85 / 255.f, 0.f };
	style_forest.c_area = { 47 / 255.f,  47 / 255.f,  47 / 255.f, 0.f };
	style_forest.c_body = { 64 / 255.f,  64 / 255.f,  64 / 255.f, 0.f };
	style_forest.c_pop = { 30 / 255.f,  30 / 255.f,  30 / 255.f, 0.f };
}

void ModuleEditor::HandleStyle()
{
	switch (current_style)
	{
	case 0: 
		style_classic.active = true;
		ImGui::StyleColorsClassic();
		break;
	
	case 1: 
		style_forest.active = true;
		SetStyle(style_forest);
		break;
	
	case 2: 
		style_blue.active = true;
		SetStyle(style_blue);
		break;
	
	case 3: 
		style_dark.active = true;
		ImGui::StyleColorsDark();
		break;
	}
}