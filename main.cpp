#include <irrlicht.h>
#include "events.h"
#include "collision_callback.h"
#include <ISceneNode.h>
#include <iostream>

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
  // Le gestionnaire d'événements
  EventReceiver receiver;
  receiver.initialize_pressed_keys();

  // Création de la fenêtre et du système de rendu.
  IrrlichtDevice *device = createDevice(iv::EDT_OPENGL,
                                        ic::dimension2d<u32>(1024, 768),
                                        16, false, false, false, &receiver);
  device->getFileSystem()->addFileArchive("data/map-20kdm2.pk3");

  iv::IVideoDriver  *driver = device->getVideoDriver();
  is::ISceneManager *smgr = device->getSceneManager();
  
  is::IAnimatedMesh *mesh_decor = smgr->getMesh("20kdm2.bsp");
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

  

  // Chargement de notre personnage (réutilisé plusieurs fois)
  is::IAnimatedMesh *mesh = smgr->getMesh("data/tris.md2");
  is::IAnimatedMesh *cyberdemon_mesh = smgr->getMesh("data/Cyber.md2");

  // Attachement de notre personnage dans la scène
  is::IAnimatedMeshSceneNode *perso = smgr->addAnimatedMeshSceneNode(mesh);
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

  /*is::ISceneNodeAnimatorCollisionResponse *anim_perso;
  anim_perso = smgr->createCollisionResponseAnimator(selector_decor,perso,
                                                radius,
                                                GRAVITY,
                                                ic::vector3df(0,0,0));*/
  
  //receiver.set_collision_response(anim_perso);

  is::IAnimatedMeshSceneNode *cyberdemon = smgr->addAnimatedMeshSceneNode(cyberdemon_mesh);
  cyberdemon->setScale(ic::vector3df(0.25,0.25,0.25));
  cyberdemon->setMaterialFlag(iv::EMF_LIGHTING, false);
  cyberdemon->setMD2Animation(is::EMAT_STAND);
  cyberdemon->setMaterialTexture(0, textures[0]);
  cyberdemon->setPosition(INITIAL_POSITION + ic::vector3df(50,0,50));
  cyberdemon->setDebugDataVisible(is::EDS_BBOX);
  cyberdemon->setName("cyberdemon");

  is::ISceneNodeAnimatorCollisionResponse *anim_cyberdemon;
  anim_cyberdemon = smgr->createCollisionResponseAnimator(selector_decor,cyberdemon,
                                                ic::vector3df(4.0,4.0,4.0),
                                                GRAVITY,
                                                ic::vector3df(0,0,0));
  cyberdemon->addAnimator(anim_cyberdemon);
  std::vector<is::IAnimatedMeshSceneNode*> ennemies;
  ennemies.push_back(cyberdemon);
  receiver.set_ennemies(ennemies);

  

  is::ITriangleSelector *selector_cyberdemon = smgr->createTriangleSelector(cyberdemon->getMesh(),cyberdemon);
  cyberdemon->setTriangleSelector(selector_cyberdemon);
  meta_selector->addTriangleSelector(selector_cyberdemon);

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

    /*std::cout << camera->getPosition().X << std::endl;
    std::cout << camera->getPosition().Y << std::endl;
    std::cout << camera->getPosition().Z << std::endl;*/
    // Dessin de la scène :
    /*if(anim_perso->isFalling())
      perso->setPosition(INITIAL_POSITION);*/
    smgr->drawAll();
    // 
    driver->endScene();
  }
  device->drop();

  return 0;
}
