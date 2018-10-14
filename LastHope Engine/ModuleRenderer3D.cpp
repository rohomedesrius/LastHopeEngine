#include "Globals.h"
#include "Application.h"
#include "ModuleRenderer3D.h"

#include "glew/include/glew.h"
#include "SDL/include/SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>

#pragma comment (lib, "glu32.lib")    /* link OpenGL Utility lib     */
#pragma comment (lib, "opengl32.lib") /* link Microsoft OpenGL lib   */
#pragma comment (lib, "glew/libx86/glew32.lib")

ModuleRenderer3D::ModuleRenderer3D(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

// Destructor
ModuleRenderer3D::~ModuleRenderer3D()
{}

// Called before render is available
bool ModuleRenderer3D::Init()
{
	LOG("Creating 3D Renderer context");
	bool ret = true;
	
	//SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

	//Create context
	context = SDL_GL_CreateContext(App->window->GetWindow());
	if(context == NULL)
	{
		LOG("OpenGL context could not be created! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	
	if(ret == true)
	{
		//Use Vsync
		if(vsync && SDL_GL_SetSwapInterval(1) < 0)
			LOG("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());

		//Initialize Projection Matrix
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		//Check for error
		GLenum error = glGetError();
		if(error != GL_NO_ERROR)
		{
			LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}

		//Check for error
		error = glGetError();
		if(error != GL_NO_ERROR)
		{
			LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}

		//Initialize glew and check for errors
		GLenum err = glewInit();
		if (err != 0)
		{
			LOG("Error initializing Glew! %s\n", glewGetErrorString(err));
			ret = false;
		}
		
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
		glClearDepth(1.0f);
		
		//Initialize clear color
		glClearColor(0.f, 0.f, 0.f, 1.f);

		//Check for error
		error = glGetError();
		if(error != GL_NO_ERROR)
		{
			LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}
		else
		{
			LOG("Renderer Specifications ...");

			LOG("	Vendor: %s", glGetString(GL_VENDOR));
			LOG("	Renderer: %s", glGetString(GL_RENDERER));
			LOG("	OpenGL version supported %s", glGetString(GL_VERSION));
			LOG("	GLSL: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

			if (err == 0)
				LOG("	Glew: %s", glewGetString(GLEW_VERSION));
		}

		//Initialize Modelview Matrix
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		GLfloat LightModelAmbient[] = {0.0f, 0.0f, 0.0f, 1.0f};
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, LightModelAmbient);
		
		lights[0].ref = GL_LIGHT0;
		lights[0].ambient.Set(0.25f, 0.25f, 0.25f, 1.0f);
		lights[0].diffuse.Set(0.75f, 0.75f, 0.75f, 1.0f);
		lights[0].SetPos(0.0f, 0.0f, 2.5f);
		lights[0].Init();
		
		GLfloat MaterialAmbient[] = {1.0f, 1.0f, 1.0f, 1.0f};
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, MaterialAmbient);

		GLfloat MaterialDiffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialDiffuse);

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		//glEnable(GL_DEPTH_TEST);
		//glEnable(GL_CULL_FACE);
		lights[0].Active(true);
		//glEnable(GL_LIGHTING);
		//glEnable(GL_COLOR_MATERIAL);
		//glEnable(GL_TEXTURE_2D);
		//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	//OnResize(App->window->screen_surface->w, App->window->screen_surface->h);
	
	//App->camera->Look(vec3(1.75f, 1.75f, 5.0f), vec3(0.0f, 0.0f, 0.0f));

	return ret;
}

bool ModuleRenderer3D::Start()
{
	bool ret = true;

	EnableDepthTest(enable_depth_test);
	EnableCullFace(enable_cull_face);
	EnableLighting(enable_lighting);
	EnableColorMaterial(enable_color_material);
	EnableTexture2D(enable_gl_texture);
	EnableWireframeMode(enable_wireframe);

	importer.SetCallback();

	// Projection matrix for
	OnResize(App->window->screen_surface->w, App->window->screen_surface->h);
	
	return ret;
}

// PreUpdate: clear buffer
update_status ModuleRenderer3D::PreUpdate(float dt)
{
	Color c = App->camera->background;
	glClearColor(c.r, c.g, c.b, c.a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(App->camera->GetViewMatrix());

	// light 0 on cam pos
	lights[0].SetPos(App->camera->position.x, App->camera->position.y, App->camera->position.z);

	for(uint i = 0; i < MAX_LIGHTS; ++i)
		lights[i].Render();
	
	return UPDATE_CONTINUE;
}

// Update: debug camera
update_status ModuleRenderer3D::Update(float dt)
{
	return UPDATE_CONTINUE;
}

// PostUpdate present buffer to screen
update_status ModuleRenderer3D::PostUpdate(float dt)
{
	// Render Configuration
	EnableDepthTest(enable_depth_test);
	EnableCullFace(enable_cull_face);
	EnableLighting(enable_lighting);
	EnableColorMaterial(enable_color_material);
	EnableTexture2D(enable_gl_texture);
	EnableWireframeMode(enable_wireframe);

	if (enable_checkers)
		LoadCheckers();

	// We just use 1 texture unit
	GLuint texture_unit = 0;
	if (enable_antisotropic)
		glBindSampler(texture_unit, g_sampler);

	for (std::vector<Mesh*>::iterator it = meshes.begin(); it != meshes.end(); it++)
	{
		if (enable_checkers)
		{
			glBindTexture(GL_TEXTURE_2D, checkers);
		}
		else
		{
			if (((*it)->buffer_texture) > 0)
			{
				glEnableClientState(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D, (*it)->buffer_texture);
			}
		}

		if (((*it)->buffer_vertex) > 0)
		{
			glEnableClientState(GL_VERTEX_ARRAY);
			glBindBuffer(GL_ARRAY_BUFFER, (*it)->buffer_vertex);
			glVertexPointer(3, GL_FLOAT, 0, NULL);
		}

		if (((*it)->buffer_normals) > 0)
		{
			glEnableClientState(GL_NORMAL_ARRAY);
			glBindBuffer(GL_ARRAY_BUFFER, (*it)->buffer_normals);
			glNormalPointer(GL_FLOAT, 0, NULL);
		}

		if (((*it)->buffer_uv) > 0)
		{
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			glBindBuffer(GL_ARRAY_BUFFER, (*it)->buffer_uv);
			glTexCoordPointer(2, GL_FLOAT, 0, NULL);
		}

		if (((*it)->buffer_index) > 0)
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, (*it)->buffer_index);
			glDrawElements(GL_TRIANGLES, (*it)->num_index, GL_UNSIGNED_INT, NULL);
		}

		// CleanUp
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		if (enable_antisotropic)
			glBindSampler(texture_unit, 0);

		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisableClientState(GL_TEXTURE_2D);

	}

	// Render Scene
	App->scene->Draw();

	// Render Engine UI
	App->editor->Draw();

	SDL_GL_SwapWindow(App->window->GetWindow());
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleRenderer3D::CleanUp()
{
	LOG("Cleaning up 3D Renderer");

	CleanScene();

	importer.CleanCallback();

	SDL_GL_DeleteContext(context);

	return true;
}

void ModuleRenderer3D::DrawUI()
{
	if (ImGui::CollapsingHeader("Renderer"))
	{
		if (ImGui::Checkbox("VSYNC", &vsync))
		{
			EnableVSync(vsync);
		}
		ImGui::Spacing(); ImGui::Spacing();
		if (ImGui::Checkbox("GL Depth Test", &enable_depth_test))
		{

		}
		if (ImGui::Checkbox("GL Cull Face", &enable_cull_face))
		{

		}
		if (ImGui::Checkbox("GL Lighting", &enable_lighting))
		{

		}
		if (ImGui::Checkbox("GL Color Material", &enable_color_material))
		{

		}
		if (ImGui::Checkbox("GL Texture 2D", &enable_gl_texture))
		{

		}
		if (ImGui::Checkbox("Use Only Checkers", &enable_checkers))
		{
			if (enable_checkers && !enable_gl_texture)
				LOG("Warning! You won't see Checkers if \"GL Texture 2D\" is disabled!");		
		}
		if (ImGui::Checkbox("Wireframe Mode", &enable_wireframe))
		{

		}
		if (ImGui::TreeNodeEx("Innovation - AF", ImGuiTreeNodeFlags_DefaultOpen))
		{
			if (ImGui::Checkbox("Antisotropic Filtering", &enable_antisotropic))
			{
				if (enable_antisotropic)
				{
					LOG("Renderer - Enabling Antisotropic Filtering. Set as \"Nearest\" (default)");
				}
				else
					LOG("Renderer - Disabling Antisotropic Filtering...");
			}
			
			if (enable_antisotropic)
			{
				if (ImGui::SmallButton(" ##afn"))
					SetSampler(0);
				ImGui::SameLine(); ImGui::Text("Nearest");
				if (ImGui::SmallButton(" ##afl"))
					SetSampler(1);
				ImGui::SameLine(); ImGui::Text("Linear");
				if (ImGui::SmallButton(" ##aflmn"))
					SetSampler(2);
				ImGui::SameLine(); ImGui::Text("Linear Mipmap Nearest");
				if (ImGui::SmallButton(" ##aflml"))
					SetSampler(3);
				ImGui::SameLine(); ImGui::Text("Linear Mipmap Linear");
				if (ImGui::SmallButton(" ##afla"))
					SetSampler(4);
				ImGui::SameLine(); ImGui::Text("Low Antisotropic");
				if (ImGui::SmallButton(" ##afma"))
					SetSampler(5);
				ImGui::SameLine(); ImGui::Text("Max Antisotropic ");
			}

			ImGui::TreePop();
		}
	}
}

void ModuleRenderer3D::DrawProperties()
{

	int a = 1;
	if (meshes.empty())
	{
		ImGui::Text("You must first load a Scene!");
	}
	else
	{
		if (ImGui::CollapsingHeader("Model", ImGuiTreeNodeFlags_DefaultOpen))
		{
			for (std::vector<Mesh*>::iterator it = meshes.begin(); it != meshes.end(); it++)
			{
				char title[40];
				sprintf_s(title, sizeof(title), "Mesh %i", a);
				if (ImGui::CollapsingHeader(title, ImGuiTreeNodeFlags_DefaultOpen))
				{
					sprintf_s(title, sizeof(title), "Transformation ##%i", a);
					if (ImGui::TreeNodeEx(title))
					{
						ImVec4 col = ImVec4(1.0f, 0.78f, 0.58f, 1.0f);

						// Transform Position
						ImGui::Text("Position: "); ImGui::SameLine();
						ImGui::PushStyleColor(ImGuiCol_Text, col);
						ImGui::Text("%.1f, %.1f, %.1f", meshes[0]->transform.position.x, meshes[0]->transform.position.y, meshes[0]->transform.position.z);
						ImGui::PopStyleColor();

						// Transform Scale
						ImGui::Text("Scale: "); ImGui::SameLine();
						ImGui::PushStyleColor(ImGuiCol_Text, col);
						ImGui::Text("%.1f, %.1f, %.1f", meshes[0]->transform.scale.x, meshes[0]->transform.scale.y, meshes[0]->transform.scale.z);
						ImGui::PopStyleColor();

						// Transform Rotation
						ImGui::Text("Rotation: "); ImGui::SameLine();
						ImGui::PushStyleColor(ImGuiCol_Text, col);
						ImGui::Text("%.1f, %.1f, %.1f", meshes[0]->transform.rotation.x, meshes[0]->transform.rotation.y, meshes[0]->transform.rotation.z);
						ImGui::PopStyleColor();


						ImGui::TreePop();
					}
					ImGui::Spacing();

					sprintf_s(title, sizeof(title), "Geometry ##%i", a);
					if (ImGui::TreeNodeEx(title, ImGuiTreeNodeFlags_DefaultOpen))
					{
						ImGui::Text("Num Index: %i", (*it)->num_index);
						ImGui::Text("Triangle Count: %i", (*it)->num_index / 3);
						ImGui::TreePop();
					}
					ImGui::Spacing();

					sprintf_s(title, sizeof(title), "Texture ##%i", a);
					if (ImGui::TreeNodeEx(title, ImGuiTreeNodeFlags_DefaultOpen))
					{
						ImVec2 texScreenPos = ImGui::GetCursorScreenPos();
						float texWidth = 200.f;
						float texHeight = 200.f;
						ImTextureID textureID = (ImTextureID)(*it)->buffer_texture;

						ImGui::Text("Currently displaying resized texture, hover to zoom");
						ImGui::Text("Dimensions: %.0fx%.0f", importer.image_dimensions.x, importer.image_dimensions.y);
						ImGui::Image(textureID, ImVec2(texWidth, texHeight), ImVec2(0, 0), ImVec2(1, 1), ImColor(255, 255, 255, 255), ImColor(255, 255, 255, 128));

						if (ImGui::IsItemHovered())
						{
							ImGui::BeginTooltip();
							float focus_sz = 32.0f;
							float focusX = ImGui::GetMousePos().x - texScreenPos.x - focus_sz * 0.5f; if (focusX < 0.0f) focusX = 0.0f; else if (focusX > texWidth - focus_sz) focusX = texWidth - focus_sz;
							float focusY = ImGui::GetMousePos().y - texScreenPos.y - focus_sz * 0.5f; if (focusY < 0.0f) focusY = 0.0f; else if (focusY > texHeight - focus_sz) focusY = texHeight - focus_sz;
							ImGui::Text("Min: (%.2f, %.2f)", focusX, focusY);
							ImGui::Text("Max: (%.2f, %.2f)", focusX + focus_sz, focusY + focus_sz);
							ImVec2 uv0 = ImVec2((focusX) / texWidth, (focusY) / texHeight);
							ImVec2 uv1 = ImVec2((focusX + focus_sz) / texWidth, (focusY + focus_sz) / texHeight);
							ImGui::Image(textureID, ImVec2(128, 128), uv0, uv1, ImColor(255, 255, 255, 255), ImColor(255, 255, 255, 128));
							ImGui::EndTooltip();
						}
						ImGui::TreePop();
					}
				}
				a++;
			}
		}
	}
}

void ModuleRenderer3D::OnResize(int width, int height)
{
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	projection_matrix = perspective(60.0f, (float)width / (float)height, 0.125f, 512.0f);
	glLoadMatrixf(&projection_matrix);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void ModuleRenderer3D::Dropped()
{
	FileExtensions extension = importer.GetExtension(App->input->GetFileDropped());

	switch (extension)
	{
	case FileExtensions::Scene3D:
	{
		LOG("Importing 3D Scene...");
		LoadFBX((char*)App->input->GetFileDropped());
		break;
	}
	case FileExtensions::Image:
	{
		LOG("Importing Image...");
		LoadImages((char*)App->input->GetFileDropped());
		break;
	}
	case FileExtensions::Unsupported:
	{
		LOG("Error: File Type not supported");
		break;
	}
	}
}

void ModuleRenderer3D::LoadFBX(char* path)
{
	CleanScene();

	std::vector<Mesh*> tmp = importer.CreateMesh(path);

	for (std::vector<Mesh*>::iterator it = tmp.begin(); it != tmp.end(); it++)
	{
		meshes.push_back(*it);
	}

	SetAABB();

	App->camera->FocusGeometry();
}

void ModuleRenderer3D::LoadImages(char * path)
{
	std::vector<Mesh*>::iterator it = meshes.begin();
	for (; it != meshes.end(); it++)
	{
		(*it)->buffer_texture = importer.LoadImageFile(path);
	}
}

void ModuleRenderer3D::CleanScene()
{
	for (std::vector<Mesh*>::iterator it = meshes.begin(); it != meshes.end(); it++)
	{
		if ((*it) != nullptr)
			delete (*it);
	}
	meshes.clear();
}

void ModuleRenderer3D::SetAABB()
{
	if (meshes.size() >= 1)
	{
		std::vector<float3> meshes_aabb_corners;

		for (int i = 0; i < meshes.size(); i++)
		{
			meshes_aabb_corners.push_back(meshes[i]->mesh_aabb.minPoint);
			meshes_aabb_corners.push_back(meshes[i]->mesh_aabb.maxPoint);
		}

		LOG("Created AABB for %i meshes", meshes_aabb_corners.size() / 2);

		model_aabb.SetNegativeInfinity();
		model_aabb.Enclose(meshes_aabb_corners.data(), meshes_aabb_corners.size());
	}
	else
		LOG("Error! there were no meshes to create an AABB!");
}

void ModuleRenderer3D::EnableVSync(bool enable)
{
	if (enable)
		SDL_GL_SetSwapInterval(1);
	else
		SDL_GL_SetSwapInterval(0);
	vsync = enable;
}

void ModuleRenderer3D::EnableDepthTest(bool enable)
{
	if (enable && !glIsEnabled(GL_DEPTH_TEST))
	{
		glEnable(GL_DEPTH_TEST);
		LOG("Renderer - Enabling Depth Test");
	}
	else if (!enable && glIsEnabled(GL_DEPTH_TEST))
	{
		glDisable(GL_DEPTH_TEST);
		LOG("Renderer - Disabling Depth Test");
	}
	enable_depth_test = enable;
}

void ModuleRenderer3D::EnableCullFace(bool enable)
{

	if (enable && !glIsEnabled(GL_CULL_FACE))
	{
		glEnable(GL_CULL_FACE);
		LOG("Renderer - Enabling Cull Face");
	}
	else if(!enable && glIsEnabled(GL_CULL_FACE))
	{
		glDisable(GL_CULL_FACE);
		LOG("Renderer - Disabling Cull Face");
	}
	enable_cull_face = enable;
}

void ModuleRenderer3D::EnableLighting(bool enable)
{
	if (enable && !glIsEnabled(GL_LIGHTING))
	{
		glEnable(GL_LIGHTING);
		LOG("Renderer - Enabling Lighting");
	}
	else if (!enable && glIsEnabled(GL_LIGHTING))
	{
		glDisable(GL_LIGHTING);
		LOG("Renderer - Disabling Lighting");
	}
	enable_lighting = enable;
}

void ModuleRenderer3D::EnableColorMaterial(bool enable)
{
	if (enable && !glIsEnabled(GL_COLOR_MATERIAL))
	{
		glEnable(GL_COLOR_MATERIAL);
		LOG("Renderer - Enabling Color Material");
	}
	else if(!enable && glIsEnabled(GL_COLOR_MATERIAL))
	{
		glDisable(GL_COLOR_MATERIAL);
		LOG("Renderer - Disabling Color Material");
	}
	enable_color_material = enable;
}

void ModuleRenderer3D::EnableTexture2D(bool enable)
{
	if (enable && !glIsEnabled(GL_TEXTURE_2D))
	{
		glEnable(GL_TEXTURE_2D);
		LOG("Renderer - Enabling Texture2D");
	}
	else if (!enable && glIsEnabled(GL_TEXTURE_2D))
	{
		glDisable(GL_TEXTURE_2D);
		LOG("Renderer - Disabling Texture2D");
	}
	enable_gl_texture = enable;
}

void ModuleRenderer3D::EnableWireframeMode(bool enable)
{
	GLint polyMode[2];
	glGetIntegerv(GL_POLYGON_MODE, polyMode);

	if (enable && polyMode[1] == GL_FILL)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		LOG("Renderer - Enabling Wireframe Mode");
	}
	else if (!enable && polyMode[1] == GL_LINE)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		LOG("Renderer - Disabling Wireframe Mode");
	}
	enable_wireframe = enable;
}

void ModuleRenderer3D::LoadCheckers()
{
	GLubyte checkImage[CHECKERS_SIZE][CHECKERS_SIZE][4];
	for (int i = 0; i < CHECKERS_SIZE; i++) {
		for (int j = 0; j < CHECKERS_SIZE; j++) {
			int c = ((((i & 0x8) == 0) ^ (((j & 0x8)) == 0))) * 255;
			checkImage[i][j][0] = (GLubyte)c;
			checkImage[i][j][1] = (GLubyte)c;
			checkImage[i][j][2] = (GLubyte)c;
			checkImage[i][j][3] = (GLubyte)255;
		}
	}

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &checkers);
	glBindTexture(GL_TEXTURE_2D, checkers);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, CHECKERS_SIZE, CHECKERS_SIZE,
		0, GL_RGBA, GL_UNSIGNED_BYTE, checkImage);

	// CleanUp
	glBindTexture(GL_TEXTURE_2D, 0);
}

void ModuleRenderer3D::SetSampler(const int number)
{
	glGenSamplers(1, &g_sampler);
	glSamplerParameteri(g_sampler, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glSamplerParameteri(g_sampler, GL_TEXTURE_WRAP_T, GL_REPEAT);

	switch (number)
	{
	case 0: // Nearest
		glSamplerParameteri(g_sampler, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glSamplerParameteri(g_sampler, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		LOG("Renderer - Set AF to \"Nearest\"");
		break;

	case 1: // Linear
		glSamplerParameteri(g_sampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glSamplerParameteri(g_sampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		LOG("Renderer - Set AF to \"Linear\"");
		break;

	case 2: // Linear Mipmap Nearest
		glSamplerParameteri(g_sampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glSamplerParameteri(g_sampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
		LOG("Renderer - Set AF to \"Linear Mipmap Nearest\"");
		break;
	
	case 3: // Linear Mipmap Linear
		glSamplerParameteri(g_sampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glSamplerParameteri(g_sampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		LOG("Renderer - Set AF to \"Linear Mipmap Linear\"");
		break;
	
	case 4: // Low Antisotropic
		glSamplerParameteri(g_sampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glSamplerParameteri(g_sampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glSamplerParameterf(g_sampler, GL_TEXTURE_MAX_ANISOTROPY_EXT, 4.0f);
		LOG("Renderer - Set AF to \"Low Antisotropic\"");
		break;
	
	case 5: // Max Antisotropic
		GLfloat maxAniso = 0.0f;
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAniso);

		glSamplerParameteri(g_sampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glSamplerParameteri(g_sampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glSamplerParameterf(g_sampler, GL_TEXTURE_MAX_ANISOTROPY_EXT, maxAniso);
		LOG("Renderer - Set AF to \"Max Antisotropic\"");
		break;
	}
}