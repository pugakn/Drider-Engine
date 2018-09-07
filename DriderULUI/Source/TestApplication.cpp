#include "TestApplication.h"
#include "DRULGPUDriver.h"
#include <dr_graphics_driver.h>
#include <dr_time.h>
#include <dr_resource_manager.h>
#include <iostream>

namespace driderSDK {

TestApplication::TestApplication()
  : platform(ultralight::Platform::instance()) {
}

TestApplication::~TestApplication() {
}

void
TestApplication::postInit() {
  Logger::startUp();
  GraphicsDriver::startUp(DR_GRAPHICS_API::D3D11,
                          m_viewport.width,
                          m_viewport.height,
                          m_hwnd);
  Time::startUp();
  ResourceManager::startUp();

  ultralight::Config config_;
  config_.face_winding = ultralight::kFaceWinding_Clockwise;
  config_.device_scale_hint = 1.0;
  config_.font_family_standard = "Segoe UI";

  platform.set_config(config_);

  ultralight::GPUDriver* gpu_driver_ = new DRULGPUDriver();

  platform.set_gpu_driver(gpu_driver_);
  //platform.set_file_system(file_system_);
  //platform.set_font_loader(font_loader_);
}

void
TestApplication::postUpdate() {
  Time::update();
}

void
TestApplication::postRender() {
  GraphicsDriver::API().clear();
  //m_editor.draw();
  GraphicsDriver::API().swapBuffers();
}
void
TestApplication::postDestroy() {
  ResourceManager::shutDown();
  Time::shutDown();
  GraphicsDriver::shutDown();
  Logger::shutDown();
}

void
TestApplication::onResize() {
  GraphicsDriver::API().resizeBackBuffer(m_viewport.width, m_viewport.height);
  std::cout << "RESIZE " << m_viewport .width<<  std::endl;
}

}