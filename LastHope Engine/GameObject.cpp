#include "GameObject.h"
#include "Application.h"

#include "CompMesh.h"
#include "CompTransform.h"

#include "JSON.h"

GameObject::GameObject()
{
	GenerateUUID(this->uuid);
}

GameObject::GameObject(std::string name, bool is_active, bool is_static, GameObject * parent)
{
	GenerateUUID(this->uuid);
	this->name = name;
	this->is_active = is_active;
	this->is_static = is_static;
	this->parent = parent;
}

GameObject::~GameObject()
{
}

GameObject * GameObject::GetParent() const
{
	return parent;
}

void GameObject::SetParent(GameObject * parent)
{
	if (parent == nullptr)
		parent = App->scene->root;

	if (this->parent != parent)
	{
		if (this->parent != nullptr)
		{
			this->parent->RemoveChild(this);
		}

		this->parent = parent;
		parent->AddChild(this);
	}
}

void GameObject::SetParentForChildren(GameObject * parent)
{
	if (parent == nullptr)
		parent = App->scene->root;

	std::vector<GameObject*>::iterator iter = children.begin();

	while (iter != children.end())
	{
		(*iter)->SetParent(parent);
		++iter;
	}
}

void GameObject::AddChild(GameObject * child)
{
	if (child != nullptr)
	{
		children.push_back(child);
	}
}

void GameObject::RemoveChild(GameObject * child)
{
	if (child != nullptr)
	{
		std::vector<GameObject*>::iterator item = children.begin();

		while (item != children.end())
		{
			if (*item == child)
			{
				children.erase(item);
				break;
			}
			++item;
		}
	}
}

void GameObject::RemoveChildren(bool clean_up)
{
	if (clean_up)
	{
		std::vector<GameObject*>::iterator item = children.begin();

		while (item != children.end())
		{
			std::vector<Component*>::iterator comp = components.begin();
			while (comp != components.end())
			{
				(*comp)->Remove();
				comp++;
			}
			(*item)->RemoveChildren(clean_up);
			item++;
		}
	}

	children.clear();
}

Component * GameObject::FindComponent(ComponentType type)
{
	if (components.empty() == false)
	{
		std::vector<Component*>::const_iterator comp = components.begin();
		while (comp != components.end())
		{
			if ((*comp)->GetType() == type)
			{
				return (*comp);
			}
			++comp;
		}
	}
	return nullptr;
}

void GameObject::AddComponent(Component * component)
{
	components.push_back(component);
}

void GameObject::DrawUI()
{
	if (ImGui::CollapsingHeader(name.c_str()))
	{
		std::vector<GameObject*>::const_iterator child_iter = children.begin();
		while (child_iter != children.end())
		{
			(*child_iter)->DrawUI();
			child_iter++;
		}
	}
}

void GameObject::Draw()
{
	App->renderer3D->DrawGameObject(this);
	std::vector<GameObject*>::const_iterator child_iter = children.begin();
	while (child_iter != children.end())
	{
		(*child_iter)->Draw();
		child_iter++;
	}
}

void GameObject::Enable()
{
	is_active = false;
}

void GameObject::Disable()
{
	is_active = false;
}

void GameObject::SetStatic(bool set_static)
{
	is_static = set_static;
}

bool GameObject::IsStatic() const
{
	return is_static;
}

bool GameObject::IsActive() const
{
	return is_active;
}

AABB GameObject::GetAABB() const
{
	return model_aabb;
}

void GameObject::SetAABB()
{
	CompMesh* comp = (CompMesh*)FindComponent(ComponentType::MESH);

	if (comp != nullptr)
	{
		std::vector<float3> meshes_aabb_corners;

		meshes_aabb_corners.push_back(comp->resource->mesh_aabb.minPoint);
		meshes_aabb_corners.push_back(comp->resource->mesh_aabb.maxPoint);

		LOG("Created AABB for %i meshes", meshes_aabb_corners.size() / 2);

		CompTransform* trans = (CompTransform*)FindComponent(ComponentType::TRANSFORM);

		model_aabb.SetNegativeInfinity();
		model_aabb.Transform(trans->GetGlobalMatrixTransf());
		model_aabb.Enclose(meshes_aabb_corners.data(), meshes_aabb_corners.size());
	}
	else
		LOG("Error! there were no meshes to create an AABB!");
}

void GameObject::GenerateUUID(std::string& uuid)
{
	for (int i = 0; i < 36; i++)
	{
		if (i == 8 || i == 13 || i == 18 || i == 23)
			uuid.push_back('-');
		else
		{
			char value = VALUES[rand() % 37];
			uuid.push_back(value);
		}
	}
}

void GameObject::SaveGO(JSONFile& scene_file)
{
	if (!children.empty())
	{
		std::vector<GameObject*>::iterator iter = children.begin();

		while (iter != children.end())
		{
			(*iter)->SaveGO(scene_file);
		}
	}
	else
	{
		//Saving main parameters
		scene_file.SetInfoString("Go.name", name.c_str());
	}
}