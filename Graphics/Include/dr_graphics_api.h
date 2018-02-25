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
  virtual ~GraphicsAPI() {};
  virtual void init(UInt32 w, UInt32 h, void* hwnd) = 0;
  virtual void destroy() = 0;
  virtual void clear() = 0;
  virtual void swapBuffers() = 0;

  Device& getDeviceReference();
  Device* getDevicePointer();

  DeviceContext& getDeviceContextReference();
  DeviceContext* getDeviceContextPointer();

  SwapChain& getSwapChainReference();
  SwapChain* getSwapChainPointer();

  RenderTarget& getBackBufferRTReference();
  RenderTarget* getBackBufferRTPointer();

  DepthStencil& getDepthStencilRTReference();
  DepthStencil* getDepthStencilRTPointer();

  Texture& getBackBufferTextureReference();
  Texture* getBackBufferTexturePointer();

  Texture& getDepthTextureReference();
  Texture* getDepthTexturePointer();

  RasterizerState& getRasterizerStateReference();
  RasterizerState* getRasterizerStatePointer();

  void* getWindowHandler();
protected:
  Device* device;
  DeviceContext* deviceContext;
  SwapChain* swapChain;
  RenderTarget* backBufferView; 
  DepthStencil* depthStencilView; 
  Texture* depthTexture;	
  Texture* backBufferTexture;
  RasterizerState* rasterizeState;
  void* m_hwnd;
};
}