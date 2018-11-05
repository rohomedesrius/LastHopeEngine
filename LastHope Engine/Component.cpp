#include "Component.h"

GameObject* Component::GetGameObject() const
{
	return game_object;
}

bool Component::IsActive() const
{
	return active;
}

void Component::SetActive(bool value)
{
	active = value;
}

ComponentType Component::GetType() const
{
	return type;
}
