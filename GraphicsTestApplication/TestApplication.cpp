#include "TestApplication.h"
#include <iostream>
#include <dr_gameObject.h>
#include <dr_d3d_swap_chain.h>
#include <dr_rasterizer_state.h>
#include <dr_radian.h>
#include <dr_quaternion.h>
#include "DrawableComponent.h"
#include "ModelDebbug.h"
#include "NPCMovement.h"
#include "StaticMeshTechnique.h"
#include "LinesTechnique.h"
#include "dr_degree.h"
#include <dr_time.h>
#include <dr_input_manager.h>
#include <dr_keyboard.h>
#include <dr_mouse.h>
#include <dr_joystick.h>

namespace driderSDK {

TestApplication::TestApplication() {}

TestApplication::~TestApplication() {}

void
TestApplication::postInit() {
  
  m_camera = std::make_shared<Camera>(_T("MAIN_CAM"), 
                                      m_viewport);

  m_camera->createProyection(45.f, 0.1f, 1000.f);


  m_camera->transform.setPosition({0.f, 100.0f, -100.0f});
  m_camera->setTarget({0.0f, 50.0f, 10.0f});
  //m_camera->transform.setRotation(Degree(180), AXIS::kX);

  ResourceManager::startUp();
  InputManager::startUp((SizeT)GetActiveWindow());
  Time::startUp();

  initResources();
  initInput();
  initSound();
  initSceneGraph();
    
  m_camera->update();

  m_sceneGraph->query(*m_camera, QUERY_ORDER::kBackToFront, 0);
  
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
  
  Node::SharedNode croc;

  if (croc = m_sceneGraph->getRoot()->getChild(_T("Croc"))) {
    
    croc->transform.rotate(Degree(90 * Time::instance().getDelta()), AXIS::kY);
    float scale = Math::cos(croc->transform.getRotation().y) * 0.5f + 1.f;
    croc->transform.setScale({scale,scale,scale}); 
  }

  if (auto node = m_sceneGraph->getRoot()->getChild(_T("Dwarf"))) {
    
    node->transform.rotate(Degree(90 * Time::instance().getDelta()), AXIS::kY);
  }
  
  Vector3D dir = m_joker->transform.getDirection();
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
  
  m_joker->transform.move((dir * f + right * s) * vel);
  
}

void
TestApplication::postUpdate() {
  //soundDriver->update();
  input();

  Time::instance().update();
  m_sceneGraph->update();
}

void TestApplication::postRender() {
  m_sceneGraph->draw();
}

void TestApplication::postDestroy() {}

void 
TestApplication::initInput() {
  
  auto rotateLeft = [&]() 
  {
    m_joker->transform.rotate(Degree(45.f), AXIS::kY);
  };

  auto rotateRight = [&]() 
  {
    m_joker->transform.rotate(Degree(-45.f), AXIS::kY);
  };

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

    node->setName(name);

    node->transform.setPosition(pos);

    node->createComponent<DrawableComponent>(*m_graphicsAPI->device, 
                                             *m_graphicsAPI->deviceContext);

    node->createComponent<ModelDebbug>(*m_graphicsAPI->device, 
                                       *m_graphicsAPI->deviceContext);

    auto drawableComponent = node->getComponent<DrawableComponent>();
    
    
    auto technique = dr_make_unique<StaticMeshTechnique>(&(*m_camera), 
                                                         &(*node));

    //drawableComponent->setModel(model);

    drawableComponent->setShaderTechnique(technique.get());

    auto debbugComponent = node->getComponent<ModelDebbug>();

    auto technique2 = dr_make_unique<LinesTechnique>(&(*m_camera), 
                                                     &(*node));

    debbugComponent->setModel(model);

    debbugComponent->setShaderTechnique(technique2.get());
    
    parent->addChild(node);

    m_techniques.push_back(std::move(technique));
    m_techniques.push_back(std::move(technique2));

    return node;
  };

  auto n = createNode(root, _T("Joker"), _T("VenomJok.X"), {0.0f, 0.0f, 0.0f});
  
  m_joker = n;

  n->addChild(m_camera);
    
  n = createNode(root, _T("Croc"), _T("Croc.X"), {150.0f, 0.0f, 200.0f});
  
  n = createNode(root, _T("Dwarf"), _T("dwarf.x"), {-100.0f, 0.0f, 300.0f});   

}
}
