#include "dr_graphics_api.h"
namespace driderSDK {
  Device & GraphicsAPI::getDeviceReference()
  {
    return *device;
  }
  Device * GraphicsAPI::getDevicePointer()
  {
    return device;
  }
  DeviceContext & GraphicsAPI::getDeviceContextReference()
  {
    return *deviceContext;
  }
  DeviceContext * GraphicsAPI::getDeviceContextPointer()
  {
    return deviceContext;
  }
  SwapChain & GraphicsAPI::getSwapChainReference()
  {
    return *swapChain;
  }
  SwapChain * GraphicsAPI::getSwapChainPointer()
  {
    return swapChain;
  }
  RenderTarget & GraphicsAPI::getBackBufferRTReference()
  {
    return *backBufferView;
  }
  RenderTarget * GraphicsAPI::getBackBufferRTPointer()
  {
    return backBufferView;
  }
  DepthStencil & GraphicsAPI::getDepthStencilRTReference()
  {
    return *depthStencilView;
  }
  DepthStencil * GraphicsAPI::getDepthStencilRTPointer()
  {
    return depthStencilView;
  }
  Texture & GraphicsAPI::getBackBufferTextureReference()
  {
    return *backBufferTexture;
  }
  Texture * GraphicsAPI::getBackBufferTexturePointer()
  {
    return backBufferTexture;
  }
  Texture & GraphicsAPI::getDepthTextureReference()
  {
    return *depthTexture;
  }
  Texture * GraphicsAPI::getDepthTexturePointer()
  {
    return depthTexture;
  }
  RasterizerState & GraphicsAPI::getRasterizerStateReference()
  {
    return *rasterizeState;
  }
  RasterizerState * GraphicsAPI::getRasterizerStatePointer()
  {
    return rasterizeState;
  }
  void * GraphicsAPI::getWindowHandler()
  {
    return m_hwnd;
  }
}