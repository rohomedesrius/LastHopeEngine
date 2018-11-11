#ifndef __GAMEOBJECT_h_
#define __GAMEOBJECT_

#include "Globals.h"
#include "Component.h"
#include "MathGeoLib/include/MathGeoLib.h"

class GameObject
{
public:

	GameObject();
	GameObject(std::string name, bool is_active, bool is_static, GameObject* parent);
	~GameObject();

	GameObject* GetParent() const;

	void SetParent(GameObject* parent = nullptr);
	void SetParentForChildren(GameObject* parent = nullptr);
	void AddChild(GameObject* child);
	void RemoveChild(GameObject* child); // Must Call SetParent() before!
	void RemoveChildren(bool clean_up = false); // Must Call SetParentForChildren() before if clean_up == true!

	Component* FindComponent(ComponentType type);
	void AddComponent(Component* component);

	void DrawUI();

	void Enable();
	void Disable();
	void SetStatic(bool set_static);

	bool IsStatic() const;
	bool IsActive() const;

	AABB getAABB() const;

	void GenerateUUID(std::string& uuid);

public:

	std::string name = "";

private:

	std::string uuid;

	AABB model_aabb;

	GameObject* parent = nullptr;
	std::vector<GameObject*> children;
	std::vector<Component*> components;

	bool is_active = true;
	bool is_static = false;
};

#endif //__GAMEOBJECT__