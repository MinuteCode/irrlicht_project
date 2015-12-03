#include "events.h"
#include <iostream>

#define CAMERA_OFFSET ic::vector3df(0,20,0)

EventReceiver::EventReceiver()
{
  collision = nullptr;
}

bool EventReceiver::OnEvent(const SEvent &event)
{
	switch(event.EventType)
	{
		case EET_KEY_INPUT_EVENT:
			return keyboard(event);

		case EET_MOUSE_INPUT_EVENT:
			return mouse(event);
		default:
			return false;
	}

	return false;
}

void EventReceiver::set_node(is::IAnimatedMeshSceneNode *n)
{
	node = n;
}

void EventReceiver::set_camera(is::ICameraSceneNode *c)
{
	camera = c;
}

void EventReceiver::set_collision_response(is::ISceneNodeAnimatorCollisionResponse *c)
{
	collision = c;
}

void EventReceiver::set_jumping(bool j)
{
	is_jumping = j;
}

bool EventReceiver::jumping()
{
	return is_jumping;
}

bool EventReceiver::moving()
{
	return is_moving;
}

std::vector<bool> EventReceiver::get_pressed_keys()
{
	return pressed_keys;
}

void EventReceiver::initialize_pressed_keys()
{
	for(int i = 0; i < 10; i++)
		pressed_keys.push_back(false);
}

bool EventReceiver::keyboard(const SEvent &event)
{

	if (event.KeyInput.PressedDown)
	{

		switch (event.KeyInput.Key)
		{
			case KEY_ESCAPE:
				exit(0);
    			break;

    		case KEY_KEY_Q:
    			pressed_keys[2] = true;
    			break;

    		case KEY_KEY_D:
    			pressed_keys[3] = true;
    			break;

    		case KEY_KEY_S:
    			if(!pressed_keys[1])
    				trigger_run_anim();
    			is_moving = true;
    			pressed_keys[1] = true;
    			break;

    		case KEY_KEY_Z:
    			if(!pressed_keys[0])
    				trigger_run_anim();
    			is_moving = true;
    			pressed_keys[0] = true;
    			break;

    		case KEY_SPACE:
    			if(!pressed_keys[4])
    				collision->jump(3.5f);
    			pressed_keys[4] = true;
    			break;

      		default:
      			break;
  		}
  		
  		
	}
	else if(!event.KeyInput.PressedDown)
	{

		switch (event.KeyInput.Key)
		{
			case KEY_KEY_Z:
				pressed_keys[0] = false;
				if(!is_jumping)
					node->setMD2Animation(is::EMAT_STAND);
				break;
			case KEY_KEY_S:
				pressed_keys[1] = false;
				if(!is_jumping)
					node->setMD2Animation(is::EMAT_STAND);
				break;
			case KEY_KEY_Q:
    			pressed_keys[2] = false;
    			break;
    		case KEY_KEY_D:
    			pressed_keys[3] = false;
    			break;


			default:;
		}
  		camera->setTarget(node->getPosition() + CAMERA_OFFSET);
	}

	return false;
}

void EventReceiver::trigger_run_anim()
{
  	node->setMD2Animation(is::EMAT_RUN);
}

bool EventReceiver::mouse(const SEvent &event)
{
	
	switch(event.MouseInput.Event)
	{

		case EMIE_MOUSE_MOVED:
		{
			ic::vector3df rotation = node->getRotation();
			rotation.Y += (event.MouseInput.X - old_x) / 2;
			old_x = event.MouseInput.X;
			node->setRotation(rotation);
			camera->setTarget(node->getPosition() + CAMERA_OFFSET);
			break;
		}

    case EMIE_LMOUSE_PRESSED_DOWN:
    {
        node->setMD2Animation(is::EMAT_ATTACK);
        for(unsigned int i = 0; i < ennemies.size(); i++)
        {
            is::IAnimatedMeshSceneNode* ennemy = ennemies[i];
            if(node->getTransformedBoundingBox().intersectsWithBox(ennemy->getTransformedBoundingBox()))
            {
              ic::vector3df rotation = node->getRotation();
              hits[i] += 1;
              ennemy->setMaterialTexture(0,textures[hits[i] % textures.size()]);
              ennemy->setPosition(ennemy->getPosition() + ic::vector3df(15*cos(rotation.Y * M_PI / 180.0f),0, -15*sin(rotation.Y * M_PI / 180.0f)));
              if(hits[i] % textures.size() == 0)
                ennemy->setMD2Animation(is::EMAT_PAIN_B);
            }
        }
        break;
    }

		default:break;
	}
	return false;
	
}


void EventReceiver::move(float x, float z)
{
	ic::vector3df position = node->getPosition();

	position.Z += z;
	position.X += x;
	node->setPosition(ic::vector3df(position.X,position.Y,position.Z));
}

void EventReceiver::check_jump_collision()
{
  if(!collision)
    return;

	if(!collision->collisionOccurred())
    {
    	if(!is_jumping)
    	{
    		node->setMD2Animation(is::EMAT_JUMP);
    		is_jumping = true;
    	}
    }

    else if(collision->collisionOccurred() && is_jumping)
    {
    	if(!pressed_keys[0] && !pressed_keys[1])
    	{
      		node->setMD2Animation(is::EMAT_STAND);
      		is_jumping = false;
      		pressed_keys[4] = false;
      	}
      	else if(is_jumping && (pressed_keys[0] || pressed_keys[1]))
      	{
      		node->setMD2Animation(is::EMAT_RUN);
      		is_jumping = false;
      		pressed_keys[4] = false;
      	}
    }

}

void EventReceiver::movement_manager()
{
	ic::vector3df rotation = node->getRotation();
	if(pressed_keys[0])
    {
      move(3*cos(rotation.Y * M_PI / 180.0f), -3*sin(rotation.Y * M_PI / 180.0f));
    }
    else if(pressed_keys[1])
    {
      move(-3*cos(rotation.Y * M_PI / 180.0f), 3*sin(rotation.Y * M_PI / 180.0f));
    }

    if(pressed_keys[2])
    {
      move(2*sin(rotation.Y * M_PI / 180.0f), 2*cos(rotation.Y * M_PI / 180.0f));
    }
    else if(pressed_keys[3])
    {
      move(-2*sin(rotation.Y * M_PI / 180.0f), -2*cos(rotation.Y * M_PI / 180.0f));
    }
    char_info = generate_char_info();
}

void EventReceiver::set_ennemies(std::vector<irr::scene::IAnimatedMeshSceneNode*> e)
{
    ennemies = e;
    for(unsigned int i = 0; i < e.size(); i++)
      hits.push_back(0);
}

void EventReceiver::set_textures(std::vector<iv::ITexture*> t)
{
  textures = t;
}

std::string EventReceiver::generate_char_info()
{
  ic::vector3df position = node->getPosition();
  std::string info = "Char position : \nX " + std::to_string(position.X) + "\nY " + std::to_string(position.Y) + "\nZ " + std::to_string(position.Z);
  for(unsigned int i = 0; i < ennemies.size(); i++)
  {
    is::IAnimatedMeshSceneNode *ennemy = ennemies[i];
    if(node->getTransformedBoundingBox().intersectsWithBox(ennemy->getTransformedBoundingBox()))
      info += "\n\nIntersection : true";
    else
      info += "\n\nIntersection : false";
  }

  return info;
}