#ifndef __RENDERER_H__
#define __RENDERER_H__

#include "Module.h"
#include "Globals.h"
#include "MathGeoLib/include/MathGeoLib.h"
#include "Light.h"
#include "Importer.h"

#include "glmath.h"

#define CHECKERS_SIZE 128
#define MAX_LIGHTS 8

class ModuleRenderer3D : public Module
{
public:
	ModuleRenderer3D(Application* app, bool start_enabled = true);
	~ModuleRenderer3D();

	bool Init();
	bool Start();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

	void DrawUI();
	void DrawProperties();

	void OnResize(int width, int height);

	void Dropped();
	void LoadFBX(char* path);
	void LoadImages(char * path);
	void CleanScene();

	void SetAABB();

public:

	void EnableVSync(bool enable);

	void EnableDepthTest(bool enable);
	void EnableCullFace(bool enable);
	void EnableLighting(bool enable);
	void EnableColorMaterial(bool enable);
	void EnableTexture2D(bool enable);
	void EnableWireframeMode(bool enable);

	Light lights[MAX_LIGHTS];
	SDL_GLContext context = nullptr;
	float3x3 normal_matrix;
	float4x4 model_matrix, view_matrix;
	mat4x4 projection_matrix;

	// MODEL
	std::vector<Mesh*> meshes = {};
	AABB model_aabb;

private:

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
	GLuint checkers = NULL;
	void LoadCheckers();
};

#endif //__RENDERER_H__