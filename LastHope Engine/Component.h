#ifndef __COMPONENT_H__
#define __COMPONENT_H__

#include "Globals.h"

enum ComponentType
{
	MESH = 0,
	TRANSFORM,
	MATERIAL,
	CAMERA
};

class GameObject;

class Component
{
public:
	Component(ComponentType _type, GameObject* game_object) : type(_type), game_object(game_object) {};

	virtual void Update() {};
	virtual void Remove() {};

	GameObject* GetGameObject() const;

	virtual void Enable() {};
	virtual void Disable() {};

	virtual void DrawUI() {}; //Not virtual

	bool IsActive() const;
	void SetActive(bool value);

	ComponentType GetType() const;

protected:

	bool active = true;
	ComponentType type;
	GameObject* game_object = nullptr;

};
#endif //__COMPONENT_H__