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

  
  static RasterizerState&
  getRasterizerState();
  
  static void* 
  getWindowHandle();

  static BlendState&
  getBlendState(DR_BLEND_STATES::E state);

  static DepthStencilState&
  getDepthStencilState(DR_DEPTH_STENCIL_STATES::E state);
 protected:

  GFXUnique<Device> m_device;
  GFXUnique<DeviceContext> m_deviceContext;
  GFXUnique<SwapChain> m_swapChain;
  GFXUnique<RenderTarget> m_backBufferView;
  GFXUnique<DepthStencil> m_depthStencilView;
  GFXUnique<RasterizerState> m_rasterizerState;

  std::vector<GFXUnique<BlendState>> m_blendSStates;
  std::vector<GFXUnique<DepthStencilState>> m_depthStencilStates;

  void* m_hwnd;
};
}