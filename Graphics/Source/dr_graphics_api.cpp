#include "dr_graphics_api.h"
#include "dr_graphics_driver.h"

namespace driderSDK {

Device&
GraphicsAPI::getDevice() {
  return *GraphicsDriver::API().m_device;
}

DeviceContext&
GraphicsAPI::getDeviceContext() {
  return *GraphicsDriver::API().m_deviceContext;
}

SwapChain&
GraphicsAPI::getSwapChain() {
  return *GraphicsDriver::API().m_swapChain;
}

RenderTarget&
GraphicsAPI::getBackBufferRT() {
  return *GraphicsDriver::API().m_backBufferView;
}

DepthStencil&
GraphicsAPI::getDepthStencil() {
  return *GraphicsDriver::API().m_depthStencilView;
}

Texture&
GraphicsAPI::getDepthTexture() {
  return *GraphicsDriver::API().m_depthTexture;
}

RasterizerState&
GraphicsAPI::getRasterizerState() {
  return *GraphicsDriver::API().m_rasterizerState;
}

void* 
GraphicsAPI::getWindowHandle() {
  return GraphicsDriver::API().m_hwnd;
}

BlendState & GraphicsAPI::getBlendStateOpaque()
{
  return *GraphicsDriver::API().m_blendSTOpaque;
}

BlendState & GraphicsAPI::getBlendStateAlphaB()
{
  return *GraphicsDriver::API().m_blendSTAlphaBlend;
}

BlendState & GraphicsAPI::getBlendStateAdditive()
{
  return *GraphicsDriver::API().m_blendSTAdditive;
}

}
