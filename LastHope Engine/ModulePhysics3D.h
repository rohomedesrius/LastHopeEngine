#pragma once
#include "Module.h"
#include "Globals.h"

#include "MathGeoLib/include/MathGeoLib.h"

// Recommended scale is 1.0f == 1 meter, no less than 0.2 objects
#define GRAVITY btVector3(0.0f, -10.0f, 0.0f) 

class DebugDrawer;

class ModulePhysics3D : public Module
{
public:
	ModulePhysics3D(Application* app, bool start_enabled = true);
	~ModulePhysics3D();

	bool Init();
	bool Start();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

	Sphere* CreateSphere(const float3 position = (float3)(0.0f, 0.0f, 0.0f), const float radius = 1.0f);
	Capsule* CreateCapsule(const float3 bottom = (float3)(0.0f, 0.0f, 0.0f), const float3 top = (float3)(0.0f, 0.0f, 0.0f), const float radius = 1.0f);

private:

	bool debug;
	//std::vector<Bodies*>list_bodies;
};