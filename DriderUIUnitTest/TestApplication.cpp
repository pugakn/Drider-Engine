#include "TestApplication.h"
#include <iostream>
#include <dr_gameObject.h>
#include <dr_d3d_swap_chain.h>
#include <dr_rasterizer_state.h>
#include <dr_time.h>
#include <dr_input_manager.h>
#include <dr_graphics_driver.h>

#ifdef DR_PLATFORM_WINDOWS
#include <dr_d3d_texture.h>
#endif
namespace driderSDK {

TestApplication::TestApplication()
  : viewport{0,0,1280, 720}, webRenderer(){
}

TestApplication::~TestApplication() {
}

void
TestApplication::onInit() {
  HWND win = GetActiveWindow();
  InputManager::startUp((size_t)win);
  GraphicsDriver::startUp(DR_GRAPHICS_API::D3D11, 
    static_cast<driderSDK::UInt32>(viewport.width),
    static_cast<driderSDK::UInt32>(viewport.height),
    win);
  InputManager* inputMngr = nullptr;
  if (InputManager::isStarted()) {
    inputMngr = InputManager::instancePtr();
  }
  quad.init();



  webRenderer.Init(1280,720);
  Time::startUp();
}
void
TestApplication::onInput() {
  InputManager::capture();
}

void
TestApplication::onUpdate() {
  Time::instance().update();
  webRenderer.update();
}

void
TestApplication::onDraw() {
  GraphicsDriver::getApiReference().clear();
  webRenderer.setTexture();
  quad.draw();
  GraphicsDriver::getApiReference().swapBuffers();
}

void
TestApplication::onDestroy() {
}

void
TestApplication::onPause() {
}

void
TestApplication::onResume() {
}


}
