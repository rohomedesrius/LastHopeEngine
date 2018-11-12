#ifndef __MATERIALIMP_H__
#define __MATERIALIMP_H__

#include "Globals.h"

class ResMaterial;

class MaterialImporter
{
public:
	MaterialImporter();
	~MaterialImporter();

	bool Init();

	bool ImportMaterial(const char* path, const char* name);
	bool SaveMaterial(const char* path);
	bool LoadMaterial(ResMaterial* path);

private:
	bool is_devil_init = false;
};
#endif