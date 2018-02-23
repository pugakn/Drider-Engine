#include "TestApplication.h"
#include <iostream>
#include <dr_gameObject.h>
#include <dr_d3d_swap_chain.h>
#include <dr_rasterizer_state.h>
#include <dr_time.h>
namespace driderSDK {

TestApplication::TestApplication()
  : viewport{0,0,1280, 720}, webRenderer(){
}

TestApplication::~TestApplication() {
}

void
TestApplication::onInit() {
  driver = new D3DGraphicsAPI;
  HWND win = GetActiveWindow();
  driver->init(static_cast<driderSDK::UInt32>(viewport.width),
               static_cast<driderSDK::UInt32>(viewport.height),
               win);
  quad.width = viewport.width;
  quad.height = viewport.height;
  quad.init(*driver->device);



  webRenderer.Init(driver,&quad);
  Time::startUp();
}
void
TestApplication::onInput() {

}

void
TestApplication::onUpdate() {
  //soundDriver->update();

  Time::instance().update();
  webRenderer.update();
}

void
TestApplication::onDraw() {
  driver->clear();
  quad.draw(*driver->deviceContext);
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
