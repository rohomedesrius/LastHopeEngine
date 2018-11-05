#include "Component.h"
#include "MathGeoLib/include/MathGeoLib.h"

class GameObject;

class CompTransform : public Component
{
public:

	CompTransform(GameObject* gameobject);
	~CompTransform();

public:

	void Remove();

	float3 local_position = { 0.f,0.f,0.f };
	float3 local_scale = { 1.f,1.f,1.f };
	Quat local_rotation = {};

private:

};