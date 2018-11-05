#include "Component.h"

class GameObject;

struct ResourceMaterial // TMP
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
	ResourceMaterial* resource = nullptr;

private:

};