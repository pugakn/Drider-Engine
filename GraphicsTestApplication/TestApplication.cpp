#include "TestApplication.h"
#include <iostream>
#include <dr_gameObject.h>
#include <dr_d3d_swap_chain.h>
#include <dr_rasterizer_state.h>
#include <dr_radian.h>
#include <dr_quaternion.h>
#include "DrawableComponent.h"
#include "StaticMeshTechnique.h"

namespace driderSDK {

TestApplication::TestApplication()
  : viewport{0,0,1280, 720},
    camera(_T("MainCamera"),
           {0, 100, 200},
           {0,0,0},
           viewport,
           45.0f,
           0.1f,
           1000.f) {
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
  m_mouseInput->capture();
  m_keyboardInput->capture();
}

void
TestApplication::onUpdate() {
  soundDriver->update();
  m_sceneGraph->update();
  camera.update(0);
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

  m_inputManager.init((size_t)win);
  
  UInt32 mouseObjID = m_inputManager.createInputObject(InputObjectType::kMouse);

  UInt32 keybObjID = m_inputManager.createInputObject(InputObjectType::kKeyboard);

  InputObject* inputObject = m_inputManager.getInputObjectByID(mouseObjID);

  m_mouseInput = dynamic_cast<MouseInput*>(inputObject);

  inputObject = m_inputManager.getInputObjectByID(keybObjID);

  m_keyboardInput = dynamic_cast<KeyboardInput*>(inputObject);
}

void 
TestApplication::initResources() {
  ResourceManager::startUp();
  ResourceManager* resourceManager = nullptr;
  if (ResourceManager::isStarted()) {
     resourceManager = &ResourceManager::instance();
  }
    
  resourceManager->loadResource(_T("axe.jpg"));

  resourceManager->loadResource(_T("VenomJok.X"));

  resourceManager->loadResource(_T("dwarf.x"));
}

void 
TestApplication::initSound() {
  soundDriver = new FMODSoundAPI;

  soundDriver->init();
  
  sound1 = new FMODSound;

  soundDriver->system->createSound("testSound.mp3",
                                   DR_SOUND_MODE::kDrMode_DEFAULT,
                                   0,
                                   sound1);
  channel = new FMODChannel;
  
  sound1->init(reinterpret_cast<SoundSystem*>(soundDriver->system->getReference()),
               reinterpret_cast<DrChannel*>(channel->getReference()));
  sound1->setMode(DR_SOUND_MODE::kDrMode_LOOP_OFF);
  sound1->play();
}

void 
TestApplication::initSceneGraph() {

  m_sceneGraph = dr_make_unique<SceneGraph>();
  m_sceneGraph->init();

  auto root = m_sceneGraph->getRoot();

  auto jokerNode = std::make_shared<GameObject>();
  jokerNode->setName(_T("Joker"));
  jokerNode->createComponent<DrawableComponent>(*driver->device, 
                                                *driver->deviceContext);

  auto inputListener = jokerNode->createComponent<InputComponent>();

  m_keyboardInput->setEventCallback(inputListener);

  auto drawableComponent = jokerNode->getComponent<DrawableComponent>();
  
  auto resourceMgr = ResourceManager::instancePtr();

  auto resource = resourceMgr->getReference(_T("dwarf.x"));

  auto model = std::dynamic_pointer_cast<Model>(resource);
  
  drawableComponent->setModel(model);

  auto technique = new StaticMeshTechnique(&camera, 
                                           &jokerNode->transform);

  drawableComponent->setShaderTechnique(technique);

  root->addChild(jokerNode);
}

}
