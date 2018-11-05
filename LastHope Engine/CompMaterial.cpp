#include "CompMaterial.h"
#include "glew/include/glew.h"

CompMaterial::CompMaterial(GameObject * gameobject) : Component(MATERIAL, gameobject)
{

}

CompMaterial::~CompMaterial()
{
	glDeleteBuffers(1, &resource->buffer_texture);
	resource->image_dimensions = ImVec2(NULL, NULL);
}

void CompMaterial::Remove()
{

}
