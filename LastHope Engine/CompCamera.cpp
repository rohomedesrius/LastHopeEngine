#include "CompCamera.h"

CompCamera::CompCamera(GameObject * gameobject) : Component(CAMERA, gameobject)
{
	frustum->NearPlaneDistance = 5;
	frustum->FarPlaneDistance = 250;
	frustum->Type = FrustumType::PerspectiveFrustum;
}

CompCamera::~CompCamera()
{

}

void CompCamera::Update()
{

}

void CompCamera::Draw()
{

}