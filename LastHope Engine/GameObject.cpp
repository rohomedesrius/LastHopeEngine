#include "GameObject.h"
#include "Application.h"


GameObject::GameObject()
{
}

GameObject::GameObject(std::string name, bool is_active, bool is_static, GameObject * parent)
{
	this->name = name;
	this->is_active = is_active;
	this->is_static = is_static;
	this->parent = parent;
}

GameObject::~GameObject()
{
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
			}
			(*item)->RemoveChildren(clean_up);
			++item;
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
		}
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
