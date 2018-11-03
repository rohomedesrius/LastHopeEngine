#ifndef __GAMEOBJECT__
#define __GAMEOBJECT__

#include "Globals.h"
#include "Component.h"

class GameObject
{
public:

	GameObject();
	GameObject(std::string name, bool is_active, bool is_static, GameObject* parent);
	~GameObject();

	void AddChild(GameObject* child);
	void RemoveChild(GameObject* child);
	void RemoveChildren();

	void AddComponent(Component*);

	void Enable();
	void Disable();
	void SetStatic(bool set_static);

public:

	std::string name = "";

private:

	GameObject* parent = nullptr;
	std::vector<GameObject*> children;
	std::vector<Component*> components;

	bool is_active = true;
	bool is_static = false;
}

#endif //__GAMEOBJECT__
