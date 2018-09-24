#include "Globals.h"
#include "Application.h"
#include "PhysBody3D.h"
#include "ModuleCamera3D.h"

ModuleCamera3D::ModuleCamera3D(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	CalculateViewMatrix();

	X = float3(1.0f, 0.0f, 0.0f);
	Y = float3(0.0f, 1.0f, 0.0f);
	Z = float3(0.0f, 0.0f, 1.0f);

	Position = float3(0.0f, 0.0f, 5.0f);
	Reference = float3(0.0f, 0.0f, 0.0f);
}

ModuleCamera3D::~ModuleCamera3D()
{}

// -----------------------------------------------------------------
bool ModuleCamera3D::Start()
{
	LOG("Setting up the camera");
	bool ret = true;

	return ret;
}

// -----------------------------------------------------------------
bool ModuleCamera3D::CleanUp()
{
	LOG("Cleaning camera");

	return true;
}

// -----------------------------------------------------------------
update_status ModuleCamera3D::Update(float dt)
{
	// Implement a debug camera with keys and mouse

	// OnKeys WASD keys -----------------------------------
	float Speed = 5.0f;

	if(App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT) Speed *= 2.0f;
	if(App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT) Speed *= 0.5f;

	float Distance = Speed * dt;

	float3 Up(0.0f, 1.0f, 0.0f);
	float3 Right = X;
	float3 Forward = Cross(Up, Right);

	Up *= Distance;
	Right *= Distance;
	Forward *= Distance;

	float3 Movement;

	if(App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) Movement += Forward;
	if(App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) Movement -= Forward;
	if(App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) Movement -= Right;
	if(App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) Movement += Right;
	if(App->input->GetKey(SDL_SCANCODE_R) == KEY_REPEAT) Movement += Up;
	if(App->input->GetKey(SDL_SCANCODE_F) == KEY_REPEAT) Movement -= Up;

	Position += Movement;
	Reference += Movement;
	
	// Mouse motion ----------------
	int dx = -App->input->GetMouseXMotion();
	int dy = -App->input->GetMouseYMotion();
	float Sensitivity = App->input->Sensitivity;

	if(App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)
	{
		Position -= Reference;

		if (dx != 0)
		{
			float DeltaX = (float)dx * Sensitivity;
			float3x3 rotate = float3x3::RotateAxisAngle(vec(0.f, 1.f, 0.f), DeltaX);

			X = rotate * X;
			Y = rotate * Y;
			Z = rotate * Z;
		}

		if (dy != 0)
		{
			float DeltaY = (float)dy * Sensitivity;
			float3x3 rotate = float3x3::RotateAxisAngle(X, DeltaY);

			Y = rotate * Y;
			Z = rotate * Z;

			if (Y.y < 0.0f)
			{
				Z = vec(0.0f, Z.y > 0.0f ? 1.0f : -1.0f, 0.0f);
				Y = Z.Cross(X);
			}
		}

		Reference = Position - Reference;
	}

	// Mouse wheel -----------------------

	Position -= Reference;

	if (dx != 0)
	{
		float DeltaX = (float)dx * Sensitivity;

		float3x3 rotate = float3x3::RotateAxisAngle(vec(0.f, 1.f, 0.f), DeltaX);

		X = rotate * X;
		Y = rotate * Y;
		Z = rotate * Z;
	}

	if (dy != 0)
	{
		float DeltaY = (float)dy * Sensitivity;
		float3x3 rotate = float3x3::RotateAxisAngle(X, DeltaY);

		Y = rotate * Y;
		Z = rotate * Z;

		if (Y.y < 0.0f)
		{
			Z = vec(0.0f, Z.y > 0.0f ? 1.0f : -1.0f, 0.0f);
			Y = Z.Cross(X);
		}
	}

	Position = Reference + Z * Position.Length();

	// Recalculate matrix -------------
	CalculateViewMatrix();

	return UPDATE_CONTINUE;
}

// -----------------------------------------------------------------
void ModuleCamera3D::Look(const float3 &Position, const float3 &Reference, bool RotateAroundReference)
{
	this->Position = Position;
	this->Reference = Reference;

	Z = (Position - Reference);
	Z.Normalize();
	X = vec(0.0f, 1.0f, 0.0f).Cross(Z).Normalized();
	Y = Z.Cross(X);

	if (!RotateAroundReference)
	{
		this->Reference = this->Position;
		this->Position += Z * 0.05f;
	}

	CalculateViewMatrix();
}

void ModuleCamera3D::LookAt(const vec & Spot)
{
	Reference = Spot;

	Z = (Position - Reference).Normalized();
	X = vec(0.0f, 1.0f, 0.0f).Cross(Z).Normalized();
	Y = Z.Cross(X);

	CalculateViewMatrix();
}

// -----------------------------------------------------------------
void ModuleCamera3D::Move(const float3 &Movement)
{
	Position += Movement;
	Reference += Movement;

	CalculateViewMatrix();
}

// -----------------------------------------------------------------
float* ModuleCamera3D::GetViewMatrixFloat()
{
	return (float*)ViewMatrix.v;
}

// -----------------------------------------------------------------
const float4x4 ModuleCamera3D::GetViewMatrix()
{
	return ViewMatrix;
}

// -----------------------------------------------------------------
void ModuleCamera3D::CalculateViewMatrix()
{
	ViewMatrix = float4x4(X.x, Y.x, Z.x, 0.0f, X.y, Y.y, Z.y, 0.0f, X.z, Y.z, Z.z, 0.0f, -Dot(X, Position), -Dot(Y, Position), -Dot(Z, Position), 1.0f);
	ViewMatrixInverse = ViewMatrix.Inverted();
}