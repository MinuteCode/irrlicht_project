#include "collision_callback.h"
#include <iostream>

bool CollisionCallback::onCollision(const irr::scene::ISceneNodeAnimatorCollisionResponse &animator)
{
	irr::scene::ISceneNode *tmp_interest_node = dynamic_cast<irr::scene::ISceneNode*>(interest_node);
	if(animator.getCollisionNode() == tmp_interest_node)
	{
		i++;
		std::cout << "c'est cool " << i << std::endl;
	}
	else
	{
		j++;
		std::cout << "c'est pas cool " << j << std::endl;
		std::cout << animator.getCollisionNode()->getName() << std::endl;
		//std::cout << interest_node->getName() << std::endl;
	}

	return false;
}

void CollisionCallback::set_interest_node(irr::scene::IAnimatedMeshSceneNode *node)
{
	interest_node = node;
}