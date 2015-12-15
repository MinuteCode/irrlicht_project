#ifndef EVENTS_H
#define EVENTS_H

#include <irrlicht.h>
#include <IAnimatedMeshSceneNode.h>
#include <IEventReceiver.h>
#include <ISceneNodeAnimatorCollisionResponse.h>
#include <ICursorControl.h>
#include <IGUIImage.h>
#include <iostream>
#include <vector>


using namespace irr;

namespace ic = irr::core;
namespace is = irr::scene;
namespace iv = irr::video;

class EventReceiver : public IEventReceiver
{

private:
	is::IAnimatedMeshSceneNode *node;
	std::vector<is::IAnimatedMeshSceneNode*> ennemies;
	std::vector<int> hits;
	is::ICameraSceneNode *camera;
	is::ISceneNodeAnimatorCollisionResponse *collision;
	is::IParticleSystemSceneNode* ps;

	std::vector<iv::ITexture*> textures;
	std::vector<ic::vector3df> locations;

	int old_x, old_y;
	int score = 0;
	bool is_moving = false;
	bool is_jumping = false;
	bool game_over = false;
	std::vector<bool> pressed_keys;
	//Z,S,Q,D,SPACE,5 autres à déterminer

	void trigger_run_anim();
	bool keyboard(const SEvent &event);
	bool mouse(const SEvent &event);
	

public:
	std::string char_info;
	EventReceiver();
	bool OnEvent(const SEvent &event);
	void set_node(is::IAnimatedMeshSceneNode *n);
	void set_camera(is::ICameraSceneNode *c);
	void set_collision_response(is::ISceneNodeAnimatorCollisionResponse *c);
	void set_particle_system(is::IParticleSystemSceneNode *particle);
	void set_locations(std::vector<ic::vector3df> loc);
	int get_score();
	bool get_game_over();
	bool jumping();
	bool moving();
	void move(float x, float z);
	void set_jumping(bool j);
	void check_jump_collision();
	void initialize_pressed_keys();
	std::vector<bool> get_pressed_keys();
	void movement_manager();
	void set_ennemies(std::vector<is::IAnimatedMeshSceneNode*> e);
	void set_textures(std::vector<iv::ITexture*> t);
	std::string generate_char_info();
};

#endif