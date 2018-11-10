#include "QuadTree.h"
#include "Application.h"
#include "GameObject.h"

// QUADNODE --------------------------------------------------------------
QuadNode::QuadNode()
{
}

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

void QuadNode::Divide()
{
	float3 centerPoint = float3::zero;
	
	float3 newCenterPoint = node_box.CenterPoint();

	for (int n = 0; n < 4; n++)
	{
		childs.push_back(QuadNode(this));
//		childs.back().SetBox(n, newCenterPoint);
	}

	std::vector<GameObject*> tmp = game_objects;
	game_objects.clear();

	for (std::vector<GameObject*>::iterator it = tmp.begin(); it != tmp.end(); it++)
	{
		AddGO(*it);
	}
}

void QuadNode::Clean()
{
	bool childsHaveChilds = false;
	std::vector<GameObject*> childsGOs;
	for (std::vector<QuadNode>::iterator it = childs.begin(); it != childs.end(); it++)
	{
		if (it->childs.empty() == false)
		{
			//If a child has childs, we shouldn't erase any of them! Just in case
			childsHaveChilds = true;
			break;
		}
		for (std::vector<GameObject*>::iterator childIt = it->game_objects.begin(); childIt != it->game_objects.end(); childIt++)
		{
			childsGOs.push_back(*childIt);
		}
	}

	if (childsHaveChilds == false)
	{
		if (childsGOs.empty() == true)
		{
			childs.clear();
		}
		else if (childsGOs.size() + game_objects.size() <= 1)
		{
			for (std::vector<GameObject*>::iterator it = childsGOs.begin(); it != childsGOs.end(); it++)
			{
				game_objects.push_back(*it);
			}
			childs.clear();
		}

		if (parent != nullptr)
		{
			parent->Clean();
		}
	}
}

AABB QuadNode::GetBox()
{
	return node_box;
}

QuadNode::~QuadNode()
{
}

//QUADTREE-----------------------------------------------------------
myQuadTree::myQuadTree()
{
}

myQuadTree::~myQuadTree()
{
}

void myQuadTree::Add(GameObject * GO)
{
	if (GO->getAABB().IsFinite())
	{
		root.AddGO(GO);
	}
}

void myQuadTree::Remove(GameObject * GO)
{
	if (GO->getAABB().IsFinite())
	{
		root.RemoveGO(GO);
	}
}


void myQuadTree::DebugDraw()
{
	root.DebugDraw();
}