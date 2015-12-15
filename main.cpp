#include <irrlicht.h>
#include "events.h"
#include "collision_callback.h"
#include <ISceneNode.h>
#include <irrString.h>
#include <iostream>
#include <ctime>

using namespace irr;

namespace ic = irr::core;
namespace is = irr::scene;
namespace iv = irr::video;

#define GRAVITY ic::vector3df(0,-9.81,0)
#define CAMERA_OFFSET ic::vector3df(0,20,0)
#define INITIAL_POSITION ic::vector3df(0,0,0)


is::ICameraSceneNode *camera;

int main()
{
  std::srand(std::time(0));
  std::vector<ic::vector3df> cyberdemons_location = {
  ic::vector3df(649.5,-14,103),
  ic::vector3df(276,114,374.5),
  ic::vector3df(-400.5,114,491.5),
  ic::vector3df(-409,114,-243),
  ic::vector3df(-403,114,468),
  ic::vector3df(625,242,1065),
  ic::vector3df(115,338,662),
  ic::vector3df(-405,274,-762.5)
  };

  // Le gestionnaire d'événements
  EventReceiver receiver;
  receiver.initialize_pressed_keys();
  receiver.set_locations(cyberdemons_location);

  // Création de la fenêtre et du système de rendu.
  IrrlichtDevice *device = createDevice(iv::EDT_OPENGL,
                                        ic::dimension2d<u32>(1024, 768),
                                        16, false, false, false, &receiver);
  device->getFileSystem()->addFileArchive("data/map-20kdm2.pk3");

  ic::stringw str = "Character position : ";
  irr::gui::IGUIEnvironment *environment = device->getGUIEnvironment();
  /*irr::gui::IGUIStaticText *text = environment->addStaticText(str.c_str(),
                                                  ic::rect<int>(20,20,320,100),
                                                  false,
                                                  true,
                                                  0,
                                                  1,
                                                  false);
  text->setOverrideColor(iv::SColor(255,255,255,255));*/

  irr::gui::IGUIImage *score_10 = environment->addImage(ic::rect<s32>(10,10,50,50)); score_10->setScaleImage(true);
  irr::gui::IGUIImage *score_1 = environment->addImage(ic::rect<s32>(50,10,90,50)); score_1->setScaleImage(true);
  iv::ITexture *digits[10];
  

  iv::IVideoDriver *driver = device->getVideoDriver();
  digits[0] = driver->getTexture("data/0.png");
  digits[1] = driver->getTexture("data/1.png");
  digits[2] = driver->getTexture("data/2.png");
  digits[3] = driver->getTexture("data/3.png");
  digits[4] = driver->getTexture("data/4.png");
  digits[5] = driver->getTexture("data/5.png");
  digits[6] = driver->getTexture("data/6.png");
  digits[7] = driver->getTexture("data/7.png");
  digits[8] = driver->getTexture("data/8.png");
  digits[9] = driver->getTexture("data/9.png");

  is::ISceneManager *smgr = device->getSceneManager();

  is::IParticleSystemSceneNode* ps = smgr->addParticleSystemSceneNode(false);
  receiver.set_particle_system(ps);
  
  is::IAnimatedMesh *mesh_decor = smgr->getMesh("20kdm2.bsp");
  mesh_decor->setMaterialFlag(iv::EMF_LIGHTING,false);

  is::IMeshSceneNode *node_decor;
  node_decor = smgr->addOctreeSceneNode(mesh_decor->getMesh(0),nullptr,-1,1024);
  node_decor->setPosition(ic::vector3df(-1300,-104,-1249));
  node_decor->setName("decor");


  std::vector<iv::ITexture*> textures;
  textures.push_back(driver->getTexture("data/cyber.jpg"));
  textures.push_back(driver->getTexture("data/cyberbpain1.jpg"));
  textures.push_back(driver->getTexture("data/cyberbpain2.jpg"));
  textures.push_back(driver->getTexture("data/cyberbpain3.jpg"));

  receiver.set_textures(textures);

  // Chargement de notre personnage (réutilisé plusieurs fois)StaticText
  is::IAnimatedMesh *mesh = smgr->getMesh("data/tris.md2");
  is::IAnimatedMesh *cyberdemon_mesh = smgr->getMesh("data/Cyber.md2");

  // Attachement de notre personnage dans la scène
  is::IAnimatedMeshSceneNode* perso = smgr->addAnimatedMeshSceneNode(mesh);
  receiver.set_node(perso);
  perso->setMaterialFlag(iv::EMF_LIGHTING, false);
  perso->setMD2Animation(is::EMAT_STAND);
  perso->setMaterialTexture(0, driver->getTexture("data/blue_texture.pcx"));
  perso->setPosition(INITIAL_POSITION);
  perso->setDebugDataVisible(is::EDS_BBOX);
  perso->setName("perso");

  //ajout de la caméra
  camera = smgr->addCameraSceneNode(perso, perso->getPosition() + ic::vector3df(-50,35,0), perso->getPosition() + ic::vector3df(0,20,0));
  receiver.set_camera(camera);

  //Génération de la physique
  is::IMetaTriangleSelector *meta_selector = smgr->createMetaTriangleSelector();
  is::ITriangleSelector *selector_decor;
  selector_decor = smgr->createOctreeTriangleSelector(node_decor->getMesh(),node_decor);
  node_decor->setTriangleSelector(selector_decor);
  meta_selector->addTriangleSelector(selector_decor);

  const ic::aabbox3d<f32>& box = perso->getBoundingBox();
        ic::vector3df radius = box.MaxEdge - box.getCenter();

  /*is::ISceneNodeAnimatorCollisionResporeceivernse *anim_perso;
  anim_perso = smgr->createCollisionResponseAnimator(selector_decor,perso,
                                                radius,
                                                GRAVITY,
                                                ic::vector3df(0,0,0));*/
  
  //receiver.set_collision_response(anim_perso);

  
  std::vector<is::IAnimatedMeshSceneNode*> ennemies;

  std::vector<is::IAnimatedMesh> cyberdemon_vector;

  is::ISceneNodeAnimatorCollisionResponse *anim_cyberdemon;

  is::IAnimatedMeshSceneNode *cyberdemon;
  is::ITriangleSelector *selector_cyberdemon;
  for(int i = 0; i < 4; i++){
    cyberdemon = smgr->addAnimatedMeshSceneNode(cyberdemon_mesh);
    cyberdemon->setScale(ic::vector3df(0.25,0.25,0.25));
    cyberdemon->setMaterialFlag(iv::EMF_LIGHTING, false);
    cyberdemon->setMD2Animation(is::EMAT_STAND);
    cyberdemon->setMaterialTexture(0, textures[0]);
    cyberdemon->setPosition(cyberdemons_location[std::rand()%8]);
    cyberdemon->setDebugDataVisible(is::EDS_BBOX);
    
    anim_cyberdemon = smgr->createCollisionResponseAnimator(selector_decor,cyberdemon,
                                                ic::vector3df(10.0,4.0,10.0),
                                                GRAVITY,
                                                ic::vector3df(0,0,0));
    cyberdemon->addAnimator(anim_cyberdemon);

    selector_cyberdemon = smgr->createTriangleSelector(cyberdemon->getMesh(), cyberdemon);
    cyberdemon->setTriangleSelector(selector_cyberdemon);
    meta_selector->addTriangleSelector(selector_cyberdemon);
    ennemies.push_back(cyberdemon);

  }
  receiver.set_ennemies(ennemies);

  /*is::IAnimatedMeshSceneNode *cyberdemon = smgr->addAnimatedMeshSceneNode(cyberdemon_mesh);
  cyberdemon->setScale(ic::vector3df(0.25,0.25,0.25));
  cyberdemon->setMaterialFlag(iv::EMF_LIGHTING, false);
  cyberdemon->setMD2Animation(is::EMAT_STAND);
  cyberdemon->setMaterialTexture(0, textures[0]);
  cyberdemon->setPosition(INITIAL_POSITION + ic::vector3df(50,0,50)ic::vector3df(649,-14,103));
  cyberdemon->setDebugDataVisible(is::EDS_BBOX);
  cyberdemon->setName("cyberdemon");

  
  cyberdemon->addAnimator(anim_cyberdemon);
  
  ennemies.push_back(cyberdemon);
  receiver.set_ennemies(ennemies);

  

  is::ITriangleSelector *selector_cyberdemon = smgr->createTriangleSelector(cyberdemon->getMesh(),cyberdemon);
  cyberdemon->setTriangleSelector(selector_cyberdemon);
  meta_selector->addTriangleSelector(selector_cyberdemon);*/

  is::ISceneNodeAnimatorCollisionResponse *anim_collision;
  anim_collision = smgr->createCollisionResponseAnimator(meta_selector,perso,
                                                radius,
                                                GRAVITY,
                                                ic::vector3df(0,0,0));

  perso->addAnimator(anim_collision);
  receiver.set_collision_response(anim_collision);

  // Un troisième personnage, qui saute en rond
  // is::IAnimatedMeshSceneNode *perso_cercle = smgr->addAnimatedMeshSceneNode(mesh);
  // perso_cercle->setMaterialFlag(iv::EMF_LIGHTING, false);
  // perso_cercle->setMD2Animation(is::EMAT_JUMP);
  // perso_cercle->setMaterialTexture(0, driver->getTexture("data/red_texture.pcx"));
  // is::ISceneNodeAnimator *anim_cercle =
  //           smgr->createFlyCircleAnimator(core::vector3df(30,0,0), 20.0f);
  // perso_cercle->addAnimator(anim_cercle);

  
  //smgr->addCameraSceneNodeFPS();
  //is::ICameraSceneNode *camera = smgr->addCameraSceneNodeMaya();


  while(device->run())
  {
    driver->beginScene(true, true, iv::SColor(100,150,200,255));
    camera->setTarget(perso->getPosition() + CAMERA_OFFSET);
    receiver.check_jump_collision();

    std::vector<bool> pressed_keys = receiver.get_pressed_keys();
    receiver.movement_manager();
    str = receiver.char_info.c_str();
    //text->setText(str.c_str());

    score_10->setImage(digits[(receiver.get_score()/10) % 10]);
    score_1->setImage(digits[(receiver.get_score()/1) % 10]);

    smgr->drawAll();
    //text->draw();
    if(receiver.get_game_over())
    {
      irr::gui::IGUIImage *win = environment->addImage(ic::rect<s32>(1024/2-200,768/2-50,1024/2+200,768/2+50)); win->setScaleImage(true);
      win->setUseAlphaChannel(true);
      win->setImage(driver->getTexture("data/you_win.png"));
    }
    environment->drawAll();
    driver->endScene();
  }
  device->drop();

  return 0;
}
