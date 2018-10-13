#pragma once

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

struct ModelTransform
{
	float3 position = {};
	float3 scale = {};
	Quat rotation = {};
};

class Mesh
{
public:

	~Mesh();

	ModelTransform transform;

	std::vector<float3> normals;
	std::vector<float3> vertex;

	uint buffNormals = 0;
	uint buffUv = 0;
	uint buffVertex = 0;
	uint buffIndex = 0;
	uint numIndex = 0;
	uint buffTexture = 0;

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

	bool bDevilInit = false;

public:

	ImVec2 imageDimensions;
};