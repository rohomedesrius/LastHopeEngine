#pragma once
#include "Module.h"
#include "Globals.h"
#include "MathGeoLib/include/MathGeoLib.h"

class ModuleCamera3D : public Module
{
public:
	ModuleCamera3D(Application* app, bool start_enabled = true);
	~ModuleCamera3D();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	// Camera Follow deleted on 2nd Commit
	void Look(const vec3 &Position, const vec3 &Reference, bool RotateAroundReference = false);
	void LookAt(const vec3 &Spot);
	void Move(const vec3 &Movement);
	float* GetViewMatrix();

	void FocusGeometry();

private:

	void CalculateViewMatrix();

public:
	
	vec3 X, Y, Z, position, reference;
	Color background;

private:

	mat4x4 view_matrix, view_matrix_inverse;
	vec3 movement, new_cam_pos;
	float camera_speed = 0.5f;
	float sensitivity = 0.25f;
};