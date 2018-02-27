#include "dr_graphics_driver.h"
namespace driderSDK {
GraphicsDriver::GraphicsDriver(DR_GRAPHICS_API::E apiType, 
                               UInt32 w, 
                               UInt32 h, 
                               void * hwnd)
{
  switch (apiType)
  {
  case DR_GRAPHICS_API::D3D11:
    m_api = dr_make_unique<D3DGraphicsAPI>();
    break;
  }

  m_api->init(w, h, hwnd);
}
GraphicsDriver::~GraphicsDriver()
{}

GraphicsAPI* 
GraphicsDriver::APIPtr() {
  return instance().m_api.get();
}

GraphicsAPI& 
GraphicsDriver::API() {
  return *APIPtr();
}

void GraphicsDriver::changeApi(DR_GRAPHICS_API::E) {}

void GraphicsDriver::onStartUp()
{}

void GraphicsDriver::onShutDown()
{
  m_api->destroy();
}
}
