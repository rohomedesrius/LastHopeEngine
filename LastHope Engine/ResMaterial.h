#ifndef __RESOURCEMATERIAL_H__
#define __RESOURCEMATERIAL_H__

#include "Resources.h"

class ResMaterial : public Resources
{
public:

	ResMaterial(uint uid);

	/*virtual ~ResourceTexture();
	bool LoadInMemory() override;
	void Save(Config& config) const override;
	void Load(const Config& config) override;
	*/
public:

	uint buffer_texture = 0;
	ImVec2 image_dimensions;
	std::string path = "";

};

#endif