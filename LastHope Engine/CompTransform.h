#include "Component.h"
#include "MathGeoLib/include/MathGeoLib.h"

class GameObject;

class CompTransform : public Component
{
public:

	CompTransform(GameObject* gameobject);
	~CompTransform();

	float3 positionLocal = { 0.f,0.f,0.f };
	float3 scaleLocal = { 1.f,1.f,1.f };

private:

};