#pragma once
#include <functional>
#include <dr_graphics_prerequisites.h>
#include <dr_memory.h>
#include <vector>
#include "dr_graphics_defines.h"
namespace driderSDK {

class Device;
class DeviceContext;
class RenderTarget;
class SwapChain;
class Texture;
class DepthStencil;
class RasterizerState;
class BlendState;
class DepthStencilState;
namespace DR_BLEND_STATES {
  enum E {
    kAlphaBlend,
    kAdditive,
    kOpaque,
    kCount
  };
}
namespace DR_DEPTH_STENCIL_STATES {
  enum E{
    kNone,
    kDepthRW,
    kDepthR,
    kCount
  };
}

class DR_GRAPHICS_EXPORT GraphicsAPI {
public:

  virtual ~GraphicsAPI() {};

  virtual 
  void init(UInt32 w, UInt32 h, void* hwnd, DR_GRAPHICS_API::E api = DR_GRAPHICS_API::D3D11);

  virtual 
  void destroy();

  virtual
  void clear();

  virtual
  void swapBuffers();

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
  getBlendState(DR_BLEND_STATES::E state);

  static DepthStencilState&
  getDepthStencilState(DR_DEPTH_STENCIL_STATES::E state);
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

  std::vector<SmartPtr<BlendState>> m_blendSStates;
  std::vector<SmartPtr<DepthStencilState>> m_depthStencilStates;;

  void* m_hwnd;
};
}