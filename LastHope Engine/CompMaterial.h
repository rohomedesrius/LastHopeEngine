#include "Component.h"

class GameObject;

struct Material
{
	uint id = NULL;

	uint buffer_texture = 0;
	ImVec2 image_dimensions;
	std::string path;
};

class CompMaterial : public Component
{
public:
	CompMaterial(GameObject* gameobject);
	~CompMaterial();

public:

	void Remove();
	
	uint material_id = NULL;
	Material* material = nullptr;

private:

};