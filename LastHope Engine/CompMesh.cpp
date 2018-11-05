#include "CompMesh.h"
#include "glew/include/glew.h"

CompMesh::CompMesh(GameObject * gameobject) : Component(MESH, gameobject)
{

}

CompMesh::~CompMesh()
{
	glDeleteBuffers(1, &mesh->buffer_normals);
	glDeleteBuffers(1, &mesh->buffer_index);
	glDeleteBuffers(1, &mesh->buffer_vertex);
	glDeleteBuffers(1, &mesh->buffer_uv);
}

void CompMesh::Remove()
{

}
