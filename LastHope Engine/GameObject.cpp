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
				// modify child's parent
				children.erase(item);
				break;
			}
			++item;
		}
	}
}

void GameObject::RemoveChildren()
{
	children.clear();
}

void GameObject::AddComponent(Component *)
{
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