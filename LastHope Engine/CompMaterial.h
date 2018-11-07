#include "Component.h"
#include "ResMaterial.h"

class GameObject;

class CompMaterial : public Component
{
public:
	CompMaterial(GameObject* gameobject);
	~CompMaterial();

public:

	void Remove();
	
	uint material_id = NULL;
	ResMaterial* resource = nullptr;

private:

};