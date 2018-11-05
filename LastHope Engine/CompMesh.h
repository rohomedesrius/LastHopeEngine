#include "Component.h"
#include "MathGeoLib/include/MathGeoLib.h"
class GameObject;

struct ResourceMesh // Temporal
{
	uint id = NULL;

	std::vector<float3> normals;
	std::vector<float3> vertex;
	std::vector<uint> index;

	uint buffer_normals = 0;
	uint buffer_uv = 0;
	uint buffer_vertex = 0;
	uint buffer_index = 0;
	uint num_index = 0;

	math::AABB mesh_aabb;
};

class CompMesh : public Component
{
public:

	CompMesh(GameObject* gameobject);
	~CompMesh();

	void Remove();

public:

	uint mesh_id = NULL;
	ResourceMesh* resource;

private:

};