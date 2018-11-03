#include "CompMaterial.h"
#include "glew/include/glew.h"

CompMaterial::CompMaterial(GameObject * gameobject) : Component(MATERIAL, gameobject)
{

}

CompMaterial::~CompMaterial()
{
	glDeleteBuffers(1, &buffer_texture);
	image_dimensions = ImVec2(NULL, NULL);
}
