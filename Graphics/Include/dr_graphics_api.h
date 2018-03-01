#pragma once
#include <functional>
#include <dr_graphics_prerequisites.h>
#include <dr_memory.h>

namespace driderSDK {

class Device;
class DeviceContext;
class RenderTarget;
class SwapChain;
class Texture;
class DepthStencil;
class RasterizerState;
class BlendState;
class DR_GRAPHICS_EXPORT GraphicsAPI {
public:
  
  virtual ~GraphicsAPI() {};

  virtual 
  void init(UInt32 w, UInt32 h, void* hwnd) = 0;

  virtual 
  void destroy() = 0;

  virtual
  void clear() = 0;

  virtual
  void swapBuffers() = 0;

  static Device& 
  getDevice();

  static DeviceContext&
  getDeviceContext();

  static SwapChain&
  getSwapChain();

  static RenderTarget&
  getBackBufferRT();

  static DepthStencil&
  getDepthStencil();

  static Texture&
  getDepthTexture();
  
  static RasterizerState&
  getRasterizerState();
  
  static void* 
  getWindowHandle();

  static BlendState&
    getBlendStateOpaque();

  static BlendState&
    getBlendStateAlphaB();

  static BlendState&
    getBlendStateAdditive();
protected:
  template<class T>
  using CustomDeleter = std::function<void(T*)>;
  
  template<class T>
  using SmartPtr = std::unique_ptr<T, CustomDeleter<T>>;

  SmartPtr<Device> m_device;
  SmartPtr<DeviceContext> m_deviceContext;
  SmartPtr<SwapChain> m_swapChain;
  SmartPtr<RenderTarget> m_backBufferView; 
  SmartPtr<DepthStencil> m_depthStencilView; 
  SmartPtr<Texture> m_depthTexture;	
  SmartPtr<RasterizerState> m_rasterizerState;

  SmartPtr<BlendState> m_blendSTAlphaBlend;
  SmartPtr<BlendState> m_blendSTOpaque;
  SmartPtr<BlendState> m_blendSTAdditive;
  void* m_hwnd;
};
}