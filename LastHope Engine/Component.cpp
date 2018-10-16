#include "Component.h"

bool Component::IsActive() const
{
	return active;
}

void Component::SetActive(bool value)
{
	active = value;
}
