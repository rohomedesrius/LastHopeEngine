#include "Component.h"

class GameObject;

class CompMaterial : public Component
{
public:
	CompMaterial(GameObject* gameobject);
	~CompMaterial();

public:

	void Remove();
	
	uint buffer_texture = 0;
	ImVec2 image_dimensions;
	std::string path;

private:

};