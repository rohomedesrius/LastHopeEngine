#include "Globals.h"
#include "Application.h"
#include "ModulePhysics3D.h"
#include "PhysBody3D.h"
#include "ModuleRenderer3D.h"

ModulePhysics3D::ModulePhysics3D(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	debug = true;
}

// Destructor
ModulePhysics3D::~ModulePhysics3D()
{

}

// Render not available yet----------------------------------
bool ModulePhysics3D::Init()
{
	LOG("Creating 3D Physics simulation");
	bool ret = true;
	
	return ret;
}

// ---------------------------------------------------------
bool ModulePhysics3D::Start()
{
	LOG("Creating Physics environment");
	/*
	world->setDebugDrawer(debug_draw);
	world->setGravity(GRAVITY);
	*/
	return true;
}

// ---------------------------------------------------------
update_status ModulePhysics3D::PreUpdate(float dt)
{
	// Step the physics world
	/*
	world->stepSimulation(dt, 15);

	// Detect collisions
	int numManifolds = world->getDispatcher()->getNumManifolds();
	for(int i = 0; i<numManifolds; i++)
	{
		btPersistentManifold* contactManifold = world->getDispatcher()->getManifoldByIndexInternal(i);
		btCollisionObject* obA = (btCollisionObject*) (contactManifold->getBody0());
		btCollisionObject* obB = (btCollisionObject*) (contactManifold->getBody1());

		int numContacts = contactManifold->getNumContacts();
		if(numContacts > 0)
		{
			PhysBody3D* pbodyA = (PhysBody3D*)obA->getUserPointer();
			PhysBody3D* pbodyB = (PhysBody3D*)obB->getUserPointer();

			if(pbodyA && pbodyB)
			{
				p2List_item<Module*>* item = pbodyA->collision_listeners.getFirst();
				while(item)
				{
					item->data->OnCollision(pbodyA, pbodyB);
					item = item->next;
				}

				item = pbodyB->collision_listeners.getFirst();
				while(item)
				{
					item->data->OnCollision(pbodyB, pbodyA);
					item = item->next;
				}
			}
		}
	}
	*/

	return UPDATE_CONTINUE;
}

// ---------------------------------------------------------
update_status ModulePhysics3D::Update(float dt)
{
	if(App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		debug = !debug;

	if(debug == true)
	{
		/*world->debugDrawWorld();

		// Render vehicles
		p2List_item<PhysVehicle3D*>* item = vehicles.getFirst();
		while(item)
		{
			item->data->Render();
			item = item->next;
		}

		// drop some primitives on 1,2,3
		if(App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
		{
			Sphere s(1);
			s.SetPos(App->camera->Position.x, App->camera->Position.y, App->camera->Position.z);
			App->physics3D->AddBody(s);
		}

		if(App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN)
		{
			Cube c(1, 1, 1);
			c.SetPos(App->camera->Position.x, App->camera->Position.y, App->camera->Position.z);
			App->physics3D->AddBody(c);
		}

		if(App->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN)
		{
			Cylinder c(0.5, 1);
			c.SetPos(App->camera->Position.x, App->camera->Position.y, App->camera->Position.z);
			App->physics3D->AddBody(c);
		}
		*/
	}

	return UPDATE_CONTINUE;
}

// ---------------------------------------------------------
update_status ModulePhysics3D::PostUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModulePhysics3D::CleanUp()
{
	LOG("Destroying 3D Physics simulation");

	// Free all the bodies ---
	/*
	for(int i = world->getNumCollisionObjects() - 1; i >= 0; i--)
	{
		btCollisionObject* obj = world->getCollisionObjectArray()[i];
		btRigidBody* body = btRigidBody::upcast(obj);
		if(body && body->getMotionState())
		{
			delete body->getMotionState();
		}
		world->removeCollisionObject(obj);
		delete obj;
	}

	// Free all collision shapes
	p2List_item<btCollisionShape*>* s_item = shapes.getFirst();
	while(s_item)
	{
		delete s_item->data;
		s_item = s_item->next;
	}
	shapes.clear();
	
	p2List_item<PhysBody3D*>* b_item = bodies.getFirst();
	while(b_item)
	{
		delete b_item->data;
		b_item = b_item->next;
	}
	bodies.clear();
	*/

	return true;
}

Sphere* ModulePhysics3D::CreateSphere(const float3 position, const float radius)
{
	Sphere ret = Sphere(position, radius);

	return &ret;
}

Capsule * ModulePhysics3D::CreateCapsule(const float3 bottom, const float3 top, const float radius)
{
	Capsule ret = Capsule(bottom, top, radius);

	return &ret;
}
