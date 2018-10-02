#pragma once
#include "Module.h"
#include "Globals.h"
#include "MathGeoLib/include/MathGeoLib.h"
#include "Light.h"

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

public:

	Light lights[MAX_LIGHTS];
	SDL_GLContext context;
	float3x3 NormalMatrix;
	float4x4 ModelMatrix, ViewMatrix;
	mat4x4 ProjectionMatrix;

private:

	bool vsync = true;

	bool enable_depth_test = true;
	bool enable_cull_face = true;
	bool enable_lighting = true;
	bool enable_color_material = true;
	bool enable_gl_texture = true;
};