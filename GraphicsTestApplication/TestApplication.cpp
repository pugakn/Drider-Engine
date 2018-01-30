#include "TestApplication.h"
#include <iostream>
#include <dr_d3d_swap_chain.h>
#include <dr_rasterizer_state.h>
#include <dr_radian.h>
#include <dr_quaternion.h>

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
  driver->init(viewport.width, viewport.height, win);

  ResourceManager::startUp();
  ResourceManager* pInstance;
  if (ResourceManager::isStarted()) {
     pInstance = &ResourceManager::instance();
  }
  
  std::vector<TString> modelsFiles{_T("VenomJok.X")};

  models.resize(modelsFiles.size());

  quad.init(*driver->device);

  for (SizeT i = 0; i < modelsFiles.size(); ++i) {
    models[i].init(*driver->device, modelsFiles[i]);
  }

  m_inputManager.init((size_t)win);
  std::cout << "mouse "
            << m_inputManager.getNumberOfDevices(InputObjectType::kMouse)
            << std::endl;
  std::cout << "joystick "
            << m_inputManager.getNumberOfDevices(InputObjectType::kJoystick)
            << std::endl;
  std::cout << "keyboard "
            << m_inputManager.getNumberOfDevices(InputObjectType::kKeyboard)
            << std::endl;
  std::cout << "unknown "
            << m_inputManager.getNumberOfDevices(InputObjectType::kUnknown)
            << std::endl;
  m_mouseInput = (MouseInput*)m_inputManager.getInputObjectByID(m_inputManager.createInputObject(InputObjectType::kMouse));
  m_mouseInput->setEventCallback(&m_mouseListener);
  
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
}

void
TestApplication::onUpdate() {
  soundDriver->update();

  for (auto& model : models) {
    //model.transform.rotate(Radian(0.005f), AXIS::kY);
    model.update();
  }

  camera.update(0);
}

void
TestApplication::onDraw() {
  driver->clear();
  //quad.draw(*driver->deviceContext, camera.getVP());
  for (auto& model : models) {
    model.draw(*driver->deviceContext, camera);
  }
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

}
