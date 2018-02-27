#include "TestApplication.h"
#include <iostream>
#include <dr_aabb_collider.h>
#include <dr_d3d_swap_chain.h>
#include <dr_degree.h>
#include <dr_gameObject.h>
#include <dr_input_manager.h>
#include <dr_joystick.h>
#include <dr_keyboard.h>
#include <dr_mouse.h>
#include <dr_quaternion.h>
#include <dr_radian.h>
#include <dr_rasterizer_state.h>
#include <dr_render_component.h>
#include <dr_time.h>
#include "DrawableComponent.h"
#include "ModelDebbug.h"
#include "NPCMovement.h"
#include "StaticMeshTechnique.h"
#include "LinesTechnique.h"

namespace driderSDK {

TestApplication::TestApplication() {}

TestApplication::~TestApplication() {}

void
TestApplication::postInit() {
  
  m_camera = std::make_shared<Camera>(_T("MAIN_CAM"), 
                                      m_viewport);

  m_camera->createProyection(45.f, 0.1f, 10000.f);
  m_camera->getTransform().setPosition({0.f, 100.0f, -100.0f});
  m_camera->setTarget({0.0f, 100.0f, 10.0f});

  m_worldCam = std::make_shared<Camera>(_T("WORLD_CAM"), 
                                        m_viewport);
  
  
  m_worldCam ->createProyection(45.f, 0.1f, 10000.f);
  m_worldCam->getTransform().setPosition({3000.f, 3000.f, -1500.f});
  m_worldCam->setTarget({0.f, 1.f, 0.f});

  m_activeCam = m_worldCam;

  m_technique = dr_make_unique<StaticMeshTechnique>();
  m_technique->compile(*m_graphicsAPI->device);

  ResourceManager::startUp();
  InputManager::startUp((SizeT)GetActiveWindow());
  Time::startUp();

  initResources();
  initInput();
  initSound();
  initSceneGraph();
  
  m_sceneGraph->getRoot()->addChild(m_worldCam);
  m_joker->addChild(m_camera);

  //m_sceneGraph->query(*m_camera, QUERY_ORDER::kBackToFront, 0);
  
  /*result = FMOD::System_Create(&system);
  result = system->getVersion(&version);
  if (version < FMOD_VERSION) {
    return;
  }

  result = system->init(32, FMOD_INIT_NORMAL, 0);
  result = system->createSound("testSound.mp3", FMOD_DEFAULT, 0, &sound1);
  result = sound1->setMode(FMOD_LOOP_OFF);*/
}
void
TestApplication::input() {
  
  InputManager::capture();
  
  GameObject::SharedGameObj croc;

  /*if (croc = m_sceneGraph->getRoot()->getChild(_T("Croc"))) {
    
    croc->transform.rotate(Degree(90 * Time::instance().getDelta()), AXIS::kY);
    float scale = Math::cos(croc->transform.getRotation().y) * 0.5f + 1.f;
    croc->transform.setScale({scale,scale,scale}); 
  }*/

  if (auto node = m_sceneGraph->getRoot()->getChild(_T("Dwarf"))) {
   
    auto nodeB = m_sceneGraph->getRoot()->findNode(_T("Joker"));

    node->getTransform().rotate(Degree(90 * Time::instance().getDelta()), AXIS::kY);
    nodeB->getTransform().rotate(Degree(90 * Time::instance().getDelta()), AXIS::kY);
  }
  
  Vector3D dir = m_joker->getTransform().getDirection();
  Vector3D right = dir.cross(Vector3D(0,1,0));
  float f = 0;
  float s = 0;

  if (Joystick::get(0)) {   
    
    f = -Joystick::get(0)->getAxis(JOYSTICK_AXIS::kLSVer);
    if (Math::abs(f) < 0.1f) {
      f = 0.0f;
    }

    s = -Joystick::get(0)->getAxis(JOYSTICK_AXIS::kLSHor);
    if (Math::abs(s) < 0.1f) {
      s = 0.0f;
    }

  }

  if (Keyboard::isKeyDown(KEY_CODE::kW)) {
    f = 1;
  }
  else if (Keyboard::isKeyDown(KEY_CODE::kS)) {
    f = -1;
  }

  if (Keyboard::isKeyDown(KEY_CODE::kA)) {
    s = 1;
  }
  else if (Keyboard::isKeyDown(KEY_CODE::kD)) {
    s = -1;
  }

  float vel = 150.f * Time::getDelta();
  
  m_joker->getTransform().move((dir * f + right * s) * vel);

}

void
TestApplication::postUpdate() {
  //soundDriver->update();
  
  Vector3D p = m_joker->getTransform().getPosition();

  input();

  Time::instance().update();

  m_sceneGraph->update();

  using SharedNode = std::shared_ptr<GameObject>;

  std::function<bool(SharedNode, SharedNode)> checkColl =
    [&](SharedNode a, SharedNode b) {

    if (a != b) {
      if (auto col = a->getComponent<AABBCollider>()) {
        auto colB = b->getComponent<AABBCollider>();

        if (col->getTransformedAABB().intersect(colB->getTransformedAABB())) {
          return true;
        }
      }

      for (auto& child : a->getChildren()) {
        if (checkColl(child, b)) {
          return true;
        }
      }  

    }

    return false;
  };
  
  /*if (checkColl(m_sceneGraph->getRoot(), m_joker) ||
      checkColl(m_sceneGraph->getRoot(), m_joker->getChild(0))) {
    m_joker->getTransform().setPosition(p);
    auto& pos = m_joker->getTransform().getPosition();
    std::cout << "Coll frame: " << pos.x << "," << pos.y << "," << pos.z << std::endl;
    m_joker->getTransform().setPosition(p);
  }  
  else 
  {    
    p = m_joker->getTransform().getPosition();  
  }*/
}

void 
TestApplication::postRender() {
  
  for (auto tech : m_techs) {
    tech->setCamera(&(*m_activeCam));  
  }

  m_sceneGraph->draw();

  m_technique->setCamera(&(*m_activeCam));

  auto dc = m_graphicsAPI->deviceContext;

  dc->setPrimitiveTopology(DR_PRIMITIVE_TOPOLOGY::kTriangleList);

  auto objects = m_sceneGraph->query(*m_camera, QUERY_ORDER::kFrontToBack, 0);

  for (auto& object : objects) {

    m_technique->setGameObject(&(*object));
    
    if (m_technique->prepareForDraw(*m_graphicsAPI->deviceContext)) {

      auto& meshes = object->getComponent<RenderComponent>()->getMeshes();

      for (auto& mesh : meshes) {
        mesh.vertexBuffer->set(*dc);
        mesh.indexBuffer->set(*dc);

        dc->draw(mesh.indicesCount, 0, 0);
      }
    }
  }

}

void 
TestApplication::postDestroy() {}

void 
TestApplication::initInput() {
  
  auto rotateLeft = [&]() 
  {
    m_joker->getTransform().rotate(Degree(45.f), AXIS::kY);
  };

  auto rotateRight = [&]() 
  {
    m_joker->getTransform().rotate(Degree(-45.f), AXIS::kY);
  };

  auto toggleCam = [&]()
  {
    if (m_activeCam == m_worldCam) {
      m_activeCam = m_camera;
    }
    else {
      m_activeCam = m_worldCam;
    }
  };

  Keyboard::addCallback(KEYBOARD_EVENT::kKeyPressed,
                        KEY_CODE::kT,
                        toggleCam);

  Keyboard::addCallback(KEYBOARD_EVENT::kKeyPressed,
                        KEY_CODE::kLEFT,
                        rotateLeft);

  Keyboard::addCallback(KEYBOARD_EVENT::kKeyPressed,
                        KEY_CODE::kRIGHT,
                        rotateRight);
}

void 
TestApplication::initResources() {
  ResourceManager* resourceManager = nullptr;
  if (ResourceManager::isStarted()) {
     resourceManager = &ResourceManager::instance();
  }
    
  resourceManager->loadResource(_T("axe.jpg"), m_graphicsAPI->device);

  resourceManager->loadResource(_T("VenomJok.X"), m_graphicsAPI->device);

  resourceManager->loadResource(_T("Croc.X"), m_graphicsAPI->device);

  resourceManager->loadResource(_T("dwarf.x"), m_graphicsAPI->device);

  resourceManager->loadResource(_T("Cube.fbx"), m_graphicsAPI->device);

  resourceManager->loadResource(_T("DuckyQuacky_.fbx"), m_graphicsAPI->device);
}

void 
TestApplication::initSound() {
  //soundDriver = new FMODSoundAPI;

  //soundDriver->init();
  //
  //sound1 = new FMODSound;

  //soundDriver->system->createSound(_T("testSound.mp3"),
  //                                 DR_SOUND_MODE::kDrMode_DEFAULT,
  //                                 0,
  //                                 sound1);
  //channel = new FMODChannel;
  //
  //sound1->init(reinterpret_cast<SoundSystem*>(soundDriver->system->getReference()),
  //             reinterpret_cast<DrChannel*>(channel->getReference()));
  //sound1->setMode(DR_SOUND_MODE::kDrMode_LOOP_OFF);
  //sound1->play();
}

void 
TestApplication::initSceneGraph() {

  m_sceneGraph = dr_make_unique<SceneGraph>();
  m_sceneGraph->init();

  auto root = m_sceneGraph->getRoot();

  //root->addChild(m_camera);

  //JoystickInput* joystickInput = nullptr;

  

  /*if (joystickInput) {
    auto inputListener = m_camera->createComponent<InputComponent>(joystickInput);
    joystickInput->setEventCallback(inputListener);
  }*/

  auto resourceMgr = ResourceManager::instancePtr();

  auto createNode = [&](std::shared_ptr<GameObject> parent, 
                        const TString& name, 
                        const TString& resName,
                        const Vector3D& pos) {
  
    auto resource = resourceMgr->getReference(resName);

    auto model = std::dynamic_pointer_cast<Model>(resource);
    
    auto node = m_sceneGraph->createNode(parent, model);

    auto p = node->createComponent<ModelDebbug>(*m_graphicsAPI->device,
                                                *m_graphicsAPI->deviceContext);

    auto tech = new LinesTechnique(&(*m_activeCam), 
                                   &(*node));

    p->setShaderTechnique(tech);

    p->setModel(model);

    m_techs.push_back(tech);
  
    node->setName(name);

    node->getTransform().setPosition(pos);

    return node;
  };

  
      
  auto n = createNode(root, _T("Croc"), _T("Croc.X"), {-200.f, 0.0f, 0.0f});
  
  m_joker = n;
  
  n = createNode(n, _T("Dwarf"), _T("dwarf.x"), {0.0f, 0.0f, 200.0f}); 

  n = createNode(root, _T("Dwarf"), _T("dwarf.x"), {200.0f, 0.0f, 600.0f}); 


  n = createNode(n, _T("Croc"), _T("Croc.X"), {0, 0.0f, 200.0f}); 

  n = createNode(n, _T("Joker"), _T("VenomJok.X"), {0.0f, 0.0f, 200.0f});

  n = createNode(root, _T("Sphere"), _T("DuckyQuacky_.fbx"), {100.f, 0.0f, 10.0f}); 

  n->getTransform().scale({50.f, 50.f, 50.f});
}
}
