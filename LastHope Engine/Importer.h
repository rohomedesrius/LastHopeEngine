#ifndef __IMPORTER_H__
#define __IMPORTER_H__

#include "Globals.h"
#include "MathGeoLib/include/MathGeoLib.h"

#include "glew/include/glew.h"
#include "SDL/include/SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>

#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"

#pragma comment (lib, "Assimp/libx86/assimp.lib")

class GameObject;
class CompMesh;
class CompMaterial;
class Resources;
struct ResourceMesh;
class ResMaterial;

enum FileExtensions
{
	Scene3D = 0,
	Image,
	Unsupported
};

class Importer
{

public:

	void SetCallback();
	void CleanCallback();

	FileExtensions GetExtension(const char* path);
	bool FileExists(const std::string& name);
	void FindAndReplace(std::string& source, std::string const& to_find, std::string const& replace);

	GameObject* ImportFBX(const char* path);
	ResMaterial* ImportTexture(const aiScene* scene, int texture_index, const char* path);
	ResourceMesh* ImportMesh(const aiScene* scene, int mesh_index);

	GLuint LoadImageFile(const char* file);

private:

	bool is_devil_init = false;

};

#endif //__IMPORTER_H__