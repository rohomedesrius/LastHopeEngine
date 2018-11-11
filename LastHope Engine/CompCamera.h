#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "Component.h"
#include "MathGeoLib/include/Geometry/Frustum.h"

enum FrustumColl
{
	contained, 
	intersection, 
	outside
};

class CompCamera : public Component
{
public:

	CompCamera(GameObject* gameobject);
	~CompCamera();

	bool is_active = true;

	void Update();

	void Draw();

private:
	Frustum* frustum;

public:
};
#endif