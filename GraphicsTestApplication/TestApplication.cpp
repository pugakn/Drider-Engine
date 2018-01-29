#include "TestApplication.h"
#include <iostream>
#include <dr_d3d_swap_chain.h>
#include <dr_rasterizer_state.h>
#include <dr_radian.h>
#include <dr_quaternion.h>

namespace driderSDK {

TestApplication::TestApplication() 
  : viewport{0,0,1280, 720},
    camera(_T("MainCamera"), {0, 100, 200}, {0,0,0}, viewport, 45.f, 0.1f, 1000.f )
{
}

TestApplication::~TestApplication()
{
}

void TestApplication::onInit()
{
  initWindow();
  driver = new D3DGraphicsAPI;
  HWND win = GetActiveWindow();
  driver->init(viewport.width, viewport.height, win );

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
  std::cout << "mouse " << m_inputManager.getNumberOfDevices(InputObjectType::kMouse) << std::endl;
  std::cout << "joystick " << m_inputManager.getNumberOfDevices(InputObjectType::kJoystick) << std::endl;
  std::cout << "keyboard " << m_inputManager.getNumberOfDevices(InputObjectType::kKeyboard) << std::endl;
  std::cout << "unknown " << m_inputManager.getNumberOfDevices(InputObjectType::kUnknown) << std::endl;
  m_mouseInput = (MouseInput*)m_inputManager.getInputObjectByID(m_inputManager.createInputObject(InputObjectType::kMouse));
  m_mouseInput->setEventCallback(&m_mouseListener);
}
void TestApplication::onInput()
{
  m_mouseInput->capture();
}
void TestApplication::onUpdate()
{
  for (auto& model : models)
  {
    //model.transform.rotate(Radian(0.005f), AXIS::kY);
    model.update();
  }
    
  camera.update(0);
}
void TestApplication::onDraw()
{
  driver->clear();
  //quad.draw(*driver->deviceContext, camera.getVP());
  for (auto& model : models)
    model.draw(*driver->deviceContext, camera);
  driver->swapBuffers();
}
void TestApplication::onDestroy()
{
}
void TestApplication::onPause()
{
}
void TestApplication::onResume()
{
}
}
