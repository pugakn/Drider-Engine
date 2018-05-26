#include "dr_graphics_driver.h"

namespace driderSDK {
GraphicsDriver::GraphicsDriver(DR_GRAPHICS_API::E apiType, 
                               UInt32 w, 
                               UInt32 h, 
                               void * hwnd)
{
  m_api.init(w, h, hwnd, apiType);
}
GraphicsDriver::~GraphicsDriver()
{}

GraphicsAPI* 
GraphicsDriver::APIPtr() {
  return &instance().m_api;
}

GraphicsAPI& 
GraphicsDriver::API() {
  return *APIPtr();
}

void GraphicsDriver::changeApi(DR_GRAPHICS_API::E api) {}

void GraphicsDriver::onStartUp()
{}

void GraphicsDriver::onShutDown()
{
  m_api.destroy();
}
}
