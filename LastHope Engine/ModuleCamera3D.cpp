#include "Globals.h"
#include "Application.h"
#include "ModuleCamera3D.h"
#include "ModuleRenderer3D.h"

ModuleCamera3D::ModuleCamera3D(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	CalculateViewMatrix();

	X = vec3(1.0f, 0.0f, 0.0f);
	Y = vec3(0.0f, 1.0f, 0.0f);
	Z = vec3(0.0f, 0.0f, 1.0f);

	position = vec3(0.0f, 3.0f, 10.0f);
	reference = vec3(0.0f, 3.0f, 0.0f);
	
}

ModuleCamera3D::~ModuleCamera3D()
{}

// -----------------------------------------------------------------
bool ModuleCamera3D::Start()
{
	LOG("Setting up the camera");
	bool ret = true;

	position = reference + Z * length(position);

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

	float camera_speed = 0.5f;
	float sens = 0.25f;

	if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT) camera_speed *= 2.0f;
	if (App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT) camera_speed *= 0.5f;

	float distance = camera_speed * dt;

	vec3 up(0.0f, 1.0f, 0.0f);
	vec3 right = X;
	vec3 forward = cross(up, right);

	up *= distance;
	right *= distance;
	forward *= distance;
	new_cam_pos = vec3(0.0f, 0.0f, 0.0f);

	//ALT rotation-----------------------
	if (App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT)
	{
		if(App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT)
		{
			FocusGeometry();

			int dx = -App->input->GetMouseXMotion();

			position -= reference;

			if (dx != 0)
			{
				float DeltaX = (float)dx * sens;

				X = rotate(X, DeltaX, vec3(0.0f, 1.0f, 0.0f));
				Y = rotate(Y, DeltaX, vec3(0.0f, 1.0f, 0.0f));
				Z = rotate(Z, DeltaX, vec3(0.0f, 1.0f, 0.0f));
			}

			position = reference + Z * length(position);
		}
		else if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)
		{
			int dx = -App->input->GetMouseXMotion() * sens;
			int dy = -App->input->GetMouseYMotion() * sens;

			if (dx != 0)
			{
				new_cam_pos += (Z * camera_speed * dx);
			}
			if (dy != 0)
			{
				new_cam_pos -= (Z * camera_speed * dy);
			}

			position += new_cam_pos;
			reference += new_cam_pos;
		}
	}
	else
	{
		if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)
		{
			//Unity-Like WSAD----------------------------------------------------------
			if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
				new_cam_pos -= Z * camera_speed;
			if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
				new_cam_pos += Z * camera_speed;
			if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) 
				new_cam_pos -= X * camera_speed;
			if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) 
				new_cam_pos += X * camera_speed;
			
			position += new_cam_pos;
			reference += new_cam_pos;
			//------------------------------------------------------------------------

			int dx = -App->input->GetMouseXMotion();
			int dy = -App->input->GetMouseYMotion();

			position -= reference;

			if (dx != 0)
			{
				float DeltaX = (float)dx * sens;

				X = rotate(X, DeltaX, vec3(0.0f, 1.0f, 0.0f));
				Y = rotate(Y, DeltaX, vec3(0.0f, 1.0f, 0.0f));
				Z = rotate(Z, DeltaX, vec3(0.0f, 1.0f, 0.0f));
			}

			if (dy != 0)
			{
				float DeltaY = (float)dy * sens;

				Y = rotate(Y, DeltaY, X);
				Z = rotate(Z, DeltaY, X);

				if (Y.y < 0.0f)
				{
					Z = vec3(0.0f, Z.y > 0.0f ? 1.0f : -1.0f, 0.0f);
					Y = cross(Z, X);
				}
			}

			position = reference + Z * length(position);
		}
	}

	//Focus on geometry-------------------
	if (App->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN)
	{
		FocusGeometry();
	}
	//------------------------------------

	//Mouse wheel Zoom-----------------------
	float zDelta = (float)App->input->GetMouseZ();

	position -= reference;

	if (zDelta < 0 && length(position) < 500.0f)
	{
		position += position * 0.1f;
	}

	if (zDelta > 0 && length(position) > 0.05f)
	{
		position -= position * 0.1f;
	}

	position += reference;
	position += movement;
	reference += movement;
	//---------------------------------

	// Recalculate matrix -------------
	CalculateViewMatrix();

	return UPDATE_CONTINUE;
}

// -----------------------------------------------------------------
void ModuleCamera3D::Look(const vec3 &Position, const vec3 &Reference, bool RotateAroundReference)
{
	this->position = Position;
	this->reference = Reference;

	Z = normalize(Position - Reference);
	X = normalize(cross(vec3(0.0f, 1.0f, 0.0f), Z));
	Y = cross(Z, X);

	if (!RotateAroundReference)
	{
		this->reference = this->position;
		this->position += Z * 0.05f;
	}

	CalculateViewMatrix();
}

// -----------------------------------------------------------------
void ModuleCamera3D::Move(const vec3 &Movement)
{
	position += Movement;
	reference += Movement;

	CalculateViewMatrix();
}

// -----------------------------------------------------------------
const float* ModuleCamera3D::GetViewMatrix()
{
	return &view_matrix;
}

// -----------------------------------------------------------------
void ModuleCamera3D::CalculateViewMatrix()
{
	view_matrix = mat4x4(X.x, Y.x, Z.x, 0.0f, X.y, Y.y, Z.y, 0.0f, X.z, Y.z, Z.z, 0.0f, -dot(X, position), -dot(Y, position), -dot(Z, position), 1.0f);
	view_matrix_inverse = inverse(view_matrix);
}

void ModuleCamera3D::FocusGeometry()
{
	vec3 box_center = vec3(App->renderer3D->model_aabb.CenterPoint().x, App->renderer3D->model_aabb.CenterPoint().y, App->renderer3D->model_aabb.CenterPoint().z);
	vec3 box_size = vec3(App->renderer3D->model_aabb.Size().x, App->renderer3D->model_aabb.Size().y, App->renderer3D->model_aabb.Size().z);
	
	reference = box_center;
	position = box_center + box_size;

	position = reference + Z * length(position);
}