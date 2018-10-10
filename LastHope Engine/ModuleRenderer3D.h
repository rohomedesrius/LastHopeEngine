#pragma once
#include "Module.h"
#include "Globals.h"
#include "MathGeoLib/include/MathGeoLib.h"
#include "Light.h"
#include "Importer.h"

#include "glmath.h"

#define MAX_LIGHTS 8

class ModuleRenderer3D : public Module
{
public:
	ModuleRenderer3D(Application* app, bool start_enabled = true);
	~ModuleRenderer3D();

	bool Init();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

	void DrawUI();

	void OnResize(int width, int height);

	void Dropped();
	void LoadMeshes(char* path);
	void LoadImages(char * path);
	void CleanScene();

public:

	void EnableVSync(bool enable);

	void EnableDepthTest(bool enable);
	void EnableCullFace(bool enable);
	void EnableLighting(bool enable);
	void EnableColorMaterial(bool enable);
	void EnableTexture2D(bool enable);
	void EnableWireframeMode(bool enable);

	Light lights[MAX_LIGHTS];
	SDL_GLContext context;
	float3x3 NormalMatrix;
	float4x4 ModelMatrix, ViewMatrix;
	mat4x4 ProjectionMatrix;

	std::vector<Mesh*> meshes;

private:

	bool once = false;

	bool vsync = true;

	bool enable_depth_test = true;
	bool enable_cull_face = false;
	bool enable_lighting = false;
	bool enable_color_material = false;
	bool enable_gl_texture = true;
	bool enable_wireframe = false;
	bool enable_checkers = false;
	bool enable_antisotropic = false;

	Importer importer;

	// Checker Texture
	GLuint checkers;
	void LoadCheckers();

	// Antisotropic Filtering
	GLuint g_sampler;
	GLuint current_sampler = 0;
	void SetSampler(const int number);
};