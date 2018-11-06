#include "CompTransform.h"
#include "GameObject.h"

CompTransform::CompTransform(GameObject* gameobject) : Component(TRANSFORM, gameobject)
{

}

CompTransform::~CompTransform()
{

}

void CompTransform::DrawUI()
{
	
}

void CompTransform::Remove()
{

}

const float3 CompTransform::GetGlobalPos()
{
	if (GetGameObject()->GetParent() != nullptr)
	{
		CompTransform* parent_transform = (CompTransform*)GetGameObject()->GetParent()->FindComponent(TRANSFORM);
		if (parent_transform != nullptr)
		{
			return local_position + parent_transform->GetGlobalPos();
		}
	}
	return local_position;
}

const float3 CompTransform::GetGlobalScale()
{
	if (GetGameObject()->GetParent() != nullptr)
	{
		CompTransform* parent_transform = (CompTransform*)GetGameObject()->GetParent()->FindComponent(TRANSFORM);
		if (parent_transform != nullptr)
		{
			float3 parent_scale = parent_transform->GetGlobalScale();
			return float3(local_scale.x * parent_scale.x, local_scale.y * parent_scale.y, local_scale.z * parent_scale.z);
		}
	}

	return local_scale;
}

const Quat CompTransform::GetGlobalRotation()
{
	if (GetGameObject()->GetParent() != nullptr)
	{
		CompTransform* parent_transform = (CompTransform*)GetGameObject()->GetParent()->FindComponent(TRANSFORM);
		if (parent_transform != nullptr)
		{
			return local_rotation * parent_transform->GetGlobalRotation();
		}
	}
	return local_rotation;
}

const float4x4 CompTransform::GetLocalMatrixTransf()
{
	return float4x4::FromTRS(local_position, local_rotation, local_scale);
}

const float4x4 CompTransform::GetGlobalMatrixTransf()
{
	return float4x4::FromTRS(GetGlobalPos(), GetGlobalRotation(), local_scale);
}

void CompTransform::SetGlobalTransform(float4x4 matrix)
{
	float3 pos;
	float3 scale;
	Quat rot;
	matrix.Decompose(pos, rot, scale);
	SetGlobalPos(pos);
	SetGlobalRotation(rot);
	SetGlobalScale(scale);
}

void CompTransform::SetGlobalPos(float3 globalPos)
{
	if (GetGameObject()->GetParent() != nullptr)
	{
		local_position = globalPos - ((CompTransform*)GetGameObject()->GetParent()->FindComponent(TRANSFORM))->GetGlobalPos();
	}
	else
	{
		local_position = globalPos;
	}
}

void CompTransform::SetGlobalScale(float3 globalScale)
{
	if (GetGameObject()->GetParent() != nullptr)
	{
		float3 parent_scale = ((CompTransform*)GetGameObject()->GetParent()->FindComponent(TRANSFORM))->GetGlobalScale();
		local_scale = float3(globalScale.x / parent_scale.x, globalScale.y / parent_scale.y, globalScale.z / parent_scale.z);
	}
	else
	{
		local_scale = globalScale;
	}
}

void CompTransform::SetGlobalRotation(Quat globalRot)
{
	if (GetGameObject()->GetParent() != nullptr)
	{
		math::Quat parent_rotation = ((CompTransform*)GetGameObject()->GetParent()->FindComponent(TRANSFORM))->GetGlobalRotation();
		local_rotation = globalRot / parent_rotation;
	}
	else
	{
		local_rotation = globalRot;
	}
}
