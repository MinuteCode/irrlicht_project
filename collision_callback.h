#ifndef COLLISION_CALLBACK_H
#define COLLISION_CALLBACK_H

#include <irrlicht.h>
#include <ISceneNodeAnimatorCollisionResponse.h>
#include <IAnimatedMeshSceneNode.h>
#include <ISceneNode.h>

class CollisionCallback : public irr::scene::ICollisionCallback
{

public:
	int i=0,j=0;
	bool onCollision(const irr::scene::ISceneNodeAnimatorCollisionResponse &animator);
	void set_interest_node(irr::scene::IAnimatedMeshSceneNode *node);

private:
	irr::scene::IAnimatedMeshSceneNode *interest_node;

};

#endif