#include "MaterialImporter.h"

#include "Devil\include\il.h"
#include "Devil\include\ilu.h"
#include "Devil\include\ilut.h"

#include "glew/include/glew.h"
#include "SDL/include/SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>

#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"

MaterialImporter::MaterialImporter()
{
}

MaterialImporter::~MaterialImporter()
{
}

bool MaterialImporter::Init()
{
	if (!is_devil_init)
	{
		ilInit();

		GLenum err = glewInit();
		if (GLEW_OK != err)
		{
			LOG("Importer - GLEW initialization error: %s", glewGetErrorString(err));
			exit(-1);
		}
		LOG("Importer - GLEW intialized successfully. Current GLEW version: %s", glewGetString(GLEW_VERSION));

		is_devil_init = true;
	}
}

bool MaterialImporter::ImportMaterial(const char* path, const char* name)
{
	bool ret = true;

	return ret;
}

bool MaterialImporter::SaveMaterial(const char* path)
{
	bool ret = true;

	return ret;
}

bool MaterialImporter::LoadMaterial(ResMaterial* path)
{
	bool ret = true;

	return ret;
}