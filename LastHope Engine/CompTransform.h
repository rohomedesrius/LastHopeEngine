#include "Component.h"
#include "MathGeoLib/include/MathGeoLib.h"

class GameObject;

class CompTransform : public Component
{
public:

	CompTransform(GameObject* gameobject);
	~CompTransform();

	void DrawUI();

	void Remove();

public:

	float3 local_position = { 0.f,0.f,0.f };
	float3 local_scale = { 1.f,1.f,1.f };
	Quat local_rotation = Quat::identity;

	const float3 GetGlobalPos();
	const float3 GetGlobalScale();
	const Quat GetGlobalRotation();
	const float4x4 GetLocalMatrixTransf();
	const float4x4 GetGlobalMatrixTransf();
	void SetGlobalTransform(float4x4 matrix);
	void SetGlobalPos(float3 globalPos);
	void SetGlobalScale(float3 globalScale);
	void SetGlobalRotation(Quat globalRot);

private:

};