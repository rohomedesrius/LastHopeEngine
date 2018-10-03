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
	void Look(const float3 &Position, const float3 &Reference, bool RotateAroundReference = false);
	void LookAt(const float3 &Spot);
	void Move(const float3 &Movement);
	float* GetViewMatrixFloat();
	const float4x4 GetViewMatrix();

private:

	void CalculateViewMatrix();

public:
	
	float3 X, Y, Z, Position, Reference;
	Color background;

private:

	float4x4 ViewMatrix, ViewMatrixInverse;
};