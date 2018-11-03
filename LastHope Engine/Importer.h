#ifndef __IMPORTER_H__
#define __IMPORTER_H__

#include "Globals.h"
#include "MathGeoLib/include/MathGeoLib.h"

#include "glew/include/glew.h"
#include "SDL/include/SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>

#pragma comment (lib, "Assimp/libx86/assimp.lib")

enum FileExtensions
{
	Scene3D = 0,
	Image,
	Unsupported
};

// To delete
struct ModelTransform
{
	float3 position = {};
	float3 scale = {};
	Quat rotation = {};
};

// To delete
class Mesh
{
public:

	~Mesh();

	ModelTransform transform;

	std::vector<float3> normals;
	std::vector<float3> vertex;

	uint buffer_normals = 0;
	uint buffer_uv = 0;
	uint buffer_vertex = 0;
	uint buffer_index = 0;
	uint num_index = 0;
	uint buffer_texture = 0;

	math::AABB mesh_aabb;

private:

};

class Importer
{

public:

	void SetCallback();
	void CleanCallback();

	FileExtensions GetExtension(const char* path);
	bool FileExists(const std::string& name);

	GLuint LoadImageFile(const char* file);
	std::vector<Mesh*> CreateMesh(const char* path);

private:

	bool is_devil_init = false;

public:

	// To delete
	ImVec2 image_dimensions;
};

#endif //__IMPORTER_H__