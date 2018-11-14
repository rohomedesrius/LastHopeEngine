#include "QuadTree.h"
#include "Application.h"
#include "GameObject.h"

// QUADNODE --------------------------------------------------------------
QuadNode::QuadNode()
{

}

QuadNode::QuadNode(float3 min, float3 max)
{
	parent = nullptr;
	node_box.minPoint = min;
	node_box.maxPoint = max;
}

QuadNode::QuadNode(QuadNode* _parent) : parent(_parent)
{
	node_box = parent->GetBox();
}

bool QuadNode::AddGO(GameObject* GO)
{
	bool ret = true;
	if (node_box.Intersects(GO->GetAABB()))
	{
		if (childs.empty())
		{
			if (game_objects.size() > 4)
			{
				DivideNode();
			}
			else
			{
				game_objects.push_back(GO);

				if (game_objects.size() == 4)
					DivideNode();
			}
				
		}
		else
		{
			//Checking which childs do collide------
			std::vector<QuadNode> chillds_colliding;
			for (std::vector<QuadNode>::iterator it = childs.begin(); it != childs.end(); it++)
			{
				if (it->node_box.Intersects(GO->GetAABB()))
				{
					chillds_colliding.push_back(&(*it));
				}
			}

			//Checking if we can push the go to any child----
			for (std::vector<QuadNode>::iterator it_2 = chillds_colliding.begin(); it_2 != chillds_colliding.end(); it_2++)
			{
				if (it_2->game_objects.size() < 4)
				{
					it_2->AddGO(GO);
					return ret;
				}
			}
		}
	}
	return ret;
}

bool QuadNode::RemoveGO(GameObject* GO)
{
	bool ret = false;

	if (!game_objects.empty())
	{
		for (std::vector<GameObject*>::iterator it = game_objects.begin(); it != game_objects.end(); it++)
		{
			if ((*it) == GO)
			{
				game_objects.erase(it);
				CleanNode();
				return true;
			}
		}
	}

	if (!childs.empty())
	{
		for (std::vector<QuadNode>::iterator it = childs.begin(); it != childs.end(); it++)
		{
			if (it->RemoveGO(GO))
			{
				return true;
			}
		}
	}

	return ret;
}

void QuadNode::DebugDraw()
{	
	App->renderer3D->DrawBox(node_box);

	for (std::vector<QuadNode>::iterator it = childs.begin(); it != childs.end(); it++)
	{
		it->DebugDraw();
	}
}

void QuadNode::DivideNode()
{
	std::vector<AABB> boxes = DivideNodeBox(this->node_box);
	float3 centerPoint = float3::zero;
	float3 newCenterPoint = node_box.CenterPoint();

	for (int i = 0; i < 4; i++)
	{
		childs.push_back(QuadNode(this));

	}

	std::vector<GameObject*> tmp = game_objects;
	game_objects.clear();

	for (std::vector<GameObject*>::iterator it = tmp.begin(); it != tmp.end(); it++)
	{
		AddGO(*it);
	}
}

std::vector<AABB> QuadNode::DivideNodeBox(AABB& node_box)
{
	std::vector<AABB> new_boxes;

	float3 main_box_center = node_box.CenterPoint();

	//Creating First AABB------

	return new_boxes;
}

void QuadNode::CleanNode()
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
			parent->CleanNode();
		}
	}
}

bool QuadNode::CheckGOs(GameObject* GO)
{
	bool ret = false;

	if (!game_objects.empty())
	{
		for (std::vector<GameObject*>::iterator it = game_objects.begin(); it != game_objects.end(); it++)
		{
			if ((*it) == GO)
				return true;
		}
	}

	if (!childs.empty())
	{
		for (std::vector<QuadNode>::iterator it = childs.begin(); it != childs.end(); it++)
		{
			if (it->CheckGOs(GO))
			{
				return true;
			}
		}
	}

	return ret;
}

AABB QuadNode::GetBox() const
{
	return node_box;
}

void QuadNode::SetBox(AABB& node_box)
{
	this->node_box = node_box;
}

QuadNode::~QuadNode()
{
}

//QUADTREE-----------------------------------------------------------
myQuadTree::myQuadTree(AABB& root_box)
{
	root.SetBox(root_box);
}

myQuadTree::~myQuadTree()
{
}

void myQuadTree::AddGO(GameObject* GO)
{
	root.AddGO(GO);
}

void myQuadTree::RemoveGO(GameObject* GO)
{
	root.RemoveGO(GO);
}

void myQuadTree::DebugDraw()
{
	root.DebugDraw();
}

bool myQuadTree::CheckGOs(GameObject* GO)
{
	return root.CheckGOs(GO);
}