#pragma once
#include <dr_graphics_prerequisites.h>
#include <memory>
namespace driderSDK {

class Device;
class DeviceContext;
class RenderTarget;
class SwapChain;
class Texture;
class DepthStencil;
class RasterizerState;
class DR_GRAPHICS_EXPORT GraphicsAPI {
public:
  virtual void init(UInt32 w, UInt32 h, void* hwnd) = 0;
  virtual void destroy() = 0;
  virtual void clear() = 0;
  virtual void swapBuffers() = 0;

  Device* device;
  DeviceContext* deviceContext;
  SwapChain* swapChain;
  RenderTarget* backBufferView; 
  DepthStencil* depthStencilView; 
  Texture* depthTexture;	
  Texture* backBufferTexture;
  RasterizerState* rasterizeState;
};
}