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

namespace driderSDK {

TestApplication::TestApplication()
  : viewport{0,0,1280, 720} {
}

TestApplication::~TestApplication() {
}

void
TestApplication::onInit() {
  initWindow();
  driver = new D3DGraphicsAPI;
  HWND win = GetActiveWindow();
  driver->init(static_cast<driderSDK::UInt32>(viewport.width),
               static_cast<driderSDK::UInt32>(viewport.height),
               win);

  m_camera = std::make_shared<Camera>(_T("MAIN_CAM"), 
                                      viewport);

  m_camera->createProyection(45.f, 0.1f, 10000.f);


  m_camera->transform.setPosition({0.f, 100.0f, -100.0f});
  m_camera->setTarget({0.0f, 50.0f, 10.0f});
  //m_camera->transform.setRotation(Degree(180), AXIS::kX);

  ResourceManager::startUp();
  InputManager::startUp(win);
  Time::startUp();

  initResources();
  initInput();
  initSound();
  initSceneGraph();

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
TestApplication::onInput() {
  
  //InputManager::instance().captureAll();
  
  Node::SharedNode croc;

  if (croc = m_sceneGraph->getRoot()->getChild(_T("Croc"))) {
    croc->transform.rotate(Degree(90 * Time::instance().getDelta()), AXIS::kY);
  }

  if (auto node = m_sceneGraph->getRoot()->getChild(_T("Dwarf"))) {

    if (croc) {
      float scale = Math::cos(croc->transform.getRotation().y) * 0.5f + 1.f;
      croc->transform.setScale({scale,scale,scale}); 
    }

    node->transform.rotate(Degree(90 * Time::instance().getDelta()), AXIS::kY);
  }

  if (auto croc = m_sceneGraph->getRoot()->getChild(_T("Croc"))) {
    croc->transform.rotate(Degree(60 * Time::instance().getDelta()), AXIS::kY);
  }

}

void
TestApplication::onUpdate() {
  //soundDriver->update();
  
  Time::instance().update();
  m_sceneGraph->update();
}

void
TestApplication::onDraw() {
  driver->clear();
  //quad.draw(*driver->deviceContext, camera.getVP());
  m_sceneGraph->draw();
  driver->swapBuffers();
}

void
TestApplication::onDestroy() {
  /*result = sound1->release();
  result = system->close();
  result = system->release();*/
}

void
TestApplication::onPause() {
}

void
TestApplication::onResume() {
}

void 
TestApplication::initInput() {

  HWND win = GetActiveWindow();

}

void 
TestApplication::initResources() {
  ResourceManager* resourceManager = nullptr;
  if (ResourceManager::isStarted()) {
     resourceManager = &ResourceManager::instance();
  }
    
  resourceManager->loadResource(_T("axe.jpg"));

  resourceManager->loadResource(_T("VenomJok.X"));

  resourceManager->loadResource(_T("Croc.X"));

  resourceManager->loadResource(_T("dwarf.x"));
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

  JoystickInput* joystickInput = nullptr;

  

  /*if (joystickInput) {
    auto inputListener = m_camera->createComponent<InputComponent>(joystickInput);
    joystickInput->setEventCallback(inputListener);
  }*/

  auto resourceMgr = ResourceManager::instancePtr();

  auto createNode = [&](std::shared_ptr<Node> parent, 
                        const TString& name, 
                        const TString& resName,
                        const Vector3D& pos) {

    auto node = std::make_shared<GameObject>();

    node->setName(name);

    node->transform.setPosition(pos);

    node->createComponent<DrawableComponent>(*driver->device, 
                                             *driver->deviceContext);

    node->createComponent<ModelDebbug>(*driver->device, 
                                       *driver->deviceContext);

    auto drawableComponent = node->getComponent<DrawableComponent>();
    
    auto resource = resourceMgr->getReference(resName);

    auto model = std::dynamic_pointer_cast<Model>(resource);
    
    auto technique = dr_make_unique<StaticMeshTechnique>(&(*m_camera), 
                                                         &(*node));

    drawableComponent->setModel(model);

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
  
  n->addChild(m_camera);
  
  auto component = n->createComponent<InputComponent>(joystickInput);

  joystickInput->setEventCallback(component);
  
  n = createNode(root, _T("Croc"), _T("Croc.X"), {150.0f, 0.0f, 0.0f});
  
  n = createNode(root, _T("Dwarf"), _T("dwarf.x"), {-100.0f, 0.0f, 0.0f});
  
}

}
