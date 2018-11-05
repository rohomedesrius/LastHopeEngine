#include "QuadTree.h"
#include "Application.h"
#include "GameObject.h"

// QUADNODE --------------------------------------------------------------
/*
QuadNode::QuadNode(float3 minPoint, float3 maxPoint) : parent(nullptr)
{
	box.minPoint = minPoint;
	box.maxPoint = maxPoint;
}
*/

QuadNode::QuadNode(QuadNode* _parent) : parent(_parent)
{
	node_box = parent->GetBox();
}

bool QuadNode::AddGO(GameObject* game_object)
{
	bool ret = false;
	if (node_box.Intersects(game_object->getAABB()))
	{
		if (childs.empty() == true)
		{
			game_objects.push_back(game_object);

			if (game_objects.size() > 1)
			{
				Divide();
			}
		}
		else
		{
			std::vector<QuadNode*> collidedWith;
			for (std::vector<QuadNode>::iterator it = childs.begin(); it != childs.end(); it++)
			{
				if (it->node_box.Intersects(game_object->getAABB()))
				{
					collidedWith.push_back(&*it);
				}
			}
			if (collidedWith.size() == 1)
			{
				collidedWith.front()->AddGO(game_object);
			}
			else if (collidedWith.size() > 1)
			{
				game_objects.push_back(game_object);
			}
		}
		ret = true;
	}
	return ret;
}

bool QuadNode::RemoveGO(GameObject *go)
{
	bool ret = false;
	if (game_objects.empty() == false)
	{
		for (std::vector<GameObject*>::iterator it = game_objects.begin(); it != game_objects.end(); it++)
		{
			if ((*it) == go)
			{
				game_objects.erase(it);
				Clean();
				return true;
			}
		}
	}

	if (childs.empty() == false)
	{
		for (std::vector<QuadNode>::iterator it = childs.begin(); it != childs.end(); it++)
		{
			ret = it->RemoveGO(go);
			if (ret == true)
			{
				break;
			}
		}
	}
	return ret;
}

void QuadNode::DebugDraw()
{
	float3 vertex[8];
	node_box.GetCornerPoints(vertex);
	
	App->renderer3D->DrawBox(vertex);

	for (std::vector<QuadNode>::iterator it = childs.begin(); it != childs.end(); it++)
	{
		it->DebugDraw();
	}

}
QuadNode::~QuadNode()
{
}
