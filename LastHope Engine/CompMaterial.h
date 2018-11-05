#include "Component.h"

class GameObject;

class CompMaterial : public Component
{
public:
	CompMaterial(GameObject* gameobject);
	~CompMaterial();

public:

	uint buffer_texture = 0;
	ImVec2 image_dimensions;
	std::string path;

private:

};