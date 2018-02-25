#include "dr_graphics_driver.h"
namespace driderSDK {
GraphicsDriver::GraphicsDriver(DR_GRAPHICS_API::E apiType, UInt32 w, UInt32 h, void * hwnd)
{
  switch (apiType)
  {
  case DR_GRAPHICS_API::D3D11:
    m_api = new D3DGraphicsAPI;
    break;
  }
  m_api->init(w, h, hwnd);
}
GraphicsDriver::~GraphicsDriver()
{
}
GraphicsAPI * GraphicsDriver::getApiPointer()
{
  return instance().m_api;
}
GraphicsAPI& GraphicsDriver::getApiReference()
{
  return *instance().m_api;
}
void GraphicsDriver::changeApi()
{
}
void GraphicsDriver::onStartUp()
{
}
void GraphicsDriver::onShutDown()
{
  delete m_api;
  m_api = nullptr;
}
}
