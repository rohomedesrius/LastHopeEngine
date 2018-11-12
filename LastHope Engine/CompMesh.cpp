#include "CompMesh.h"
#include "glew/include/glew.h"

CompMesh::CompMesh(GameObject * gameobject) : Component(MESH, gameobject)
{

}

CompMesh::~CompMesh()
{
	glDeleteBuffers(1, &resource->buffer_normals);
	glDeleteBuffers(1, &resource->buffer_index);
	glDeleteBuffers(1, &resource->buffer_vertex);
	glDeleteBuffers(1, &resource->buffer_uv);
}

void CompMesh::Remove()
{

}