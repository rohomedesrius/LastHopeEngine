#ifndef _QUADTREE_H_
#define _QUADTREE_H_

#include "MathGeoLib/include/MathGeoLib.h"

class GameObject;

class QuadNode
{
public:
	//QuadNode(float3 minPoint, float3 maxPoint);
	QuadNode(QuadNode* parent);
	~QuadNode();

private:
	QuadNode* parent;
	std::vector<QuadNode> childs;
	std::vector<GameObject*> game_objects;

	AABB node_box;

public:
	bool AddGO(GameObject* game_obj);
	bool RemoveGO(GameObject* game_obj);
	
	template <typename TYPE>
	std::vector<GameObject*> FilterCollisions(TYPE col);

	void DebugDraw();

	//AABB GetBox();
	//void SetBox(int n, float3 breakPoint);

private:
	void Divide();
	void Clean();
};

class myQuadTree
{
public:
	myQuadTree(float3 minPoint, float3 maxPoint);
	~myQuadTree();

	void Add(GameObject* GO);
	void Remove(GameObject* GO);

	template <typename TYPE>
	//std::vector<GameObject*> FilterCollisions(TYPE col);

	void DebugDraw();

private:
	QuadNode root;
};

template<typename TYPE>
inline std::vector<GameObject*> QuadNode::FilterCollisions(TYPE col)
{
	std::vector<GameObject*> ret;
	if (box.Intersects(col))
	{
		if (GOs.empty() == false)
		{
			for (std::vector<GameObject*>::iterator it = GOs.begin(); it != GOs.end(); it++)
			{
				if ((*it)->IsActive() && (*it)->aabb.IsFinite() && col.Intersects((*it)->aabb) == true)
				{
					ret.push_back(*it);
				}
			}
		}
		if (childs.empty() == false)
		{
			for (std::vector<QuadNode>::iterator it = childs.begin(); it != childs.end(); it++)
			{
				std::vector<GameObject*> toAdd = it->FilterCollisions(col);
				if (toAdd.empty() == false)
				{
					for (std::vector<GameObject*>::iterator it = toAdd.begin(); it != toAdd.end(); it++)
					{
						ret.push_back(*it);
					}
				}
			}
		}
	}
	return ret;
}

#endif