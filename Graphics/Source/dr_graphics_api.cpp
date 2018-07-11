#include "dr_graphics_api.h"
#include "dr_graphics_driver.h"
#include "dr_device_context.h"
#include "dr_swap_chain.h"
#include "dr_render_target.h"
#include "dr_rasterizer_state.h"
#include "dr_depth_stencil_state.h"
#include "dr_blend_state.h"
#include "dr_depth_stencil.h"
#include <dr_memory.h>

#if DR_PLATFORM  == DR_PLATFORM_WINDOWS
#include <dxgi.h>
#include <d3d11.h>
#include <DirectXMath.h>
#include "dr_d3d_texture.h"
#include "dr_d3d_device.h"
#include "dr_d3d_device_context.h"
#endif
namespace driderSDK {

void 
GraphicsAPI::init(UInt32 w, UInt32 h, void* hwnd, DR_GRAPHICS_API::E api) {
  if (api == DR_GRAPHICS_API::D3D11) {
#if DR_PLATFORM  == DR_PLATFORM_WINDOWS
    m_device = dr_gfx_unique<Device>(new D3DDevice);
    m_deviceContext = dr_gfx_unique<DeviceContext>(new D3DDeviceContext);
#else
    exit(666);
#endif
  }
  else {
    exit(666);
  }

  m_hwnd = hwnd;
  m_device->createDeviceAndDeviceContext(*m_deviceContext);

  //	Descriptor of the Swap Chain
  DrSwapChainDesc swapDesc;
  swapDesc.bufferCount = 1;
  swapDesc.windowed = true;
  swapDesc.windowHandler = hwnd;
  swapDesc.height = h;
  swapDesc.width = w;

  m_swapChain = dr_gfx_unique(m_device->createSwapChain(swapDesc));

  DrDepthStencilDesc depthTextureDesc;
  depthTextureDesc.bindFlags = DR_BIND_FLAGS::DEPTH_STENCIL | DR_BIND_FLAGS::SHADER_RESOURCE;
  depthTextureDesc.width = w;
  depthTextureDesc.height = h;
  depthTextureDesc.Format = DR_FORMAT::kD24_UNORM_S8_UINT;

  {
    auto ds = m_device->createDepthStencil(depthTextureDesc);
    m_depthStencilView = dr_gfx_unique(ds);
  }

  m_swapChain->getBackBufferRT().set(*m_deviceContext, *m_depthStencilView);

  DrRasterizerDesc rasterizerStateDesc;
  rasterizerStateDesc.fillMode = DR_FILL_MODE::kSolid;
  rasterizerStateDesc.cullMode = DR_CULL_MODE::kBack;
  rasterizerStateDesc.frontCounterClockwise = false;
  rasterizerStateDesc.depthBias = 0;
  rasterizerStateDesc.depthBiasClamp = 0.0f;
  rasterizerStateDesc.slopeScaledDepthBias = 0.0f;
  rasterizerStateDesc.depthClipEnable = true;
  rasterizerStateDesc.scissorEnable = false;
  rasterizerStateDesc.multisampleEnable = false;
  rasterizerStateDesc.antialiasedLineEnable = false;
  {
    auto rs = m_device->createRasteizerState(rasterizerStateDesc);

    m_rasterizerState = dr_gfx_unique(rs);
  }

  m_rasterizerState->set(*m_deviceContext);


  m_blendSStates.resize(DR_BLEND_STATES::kCount);
  m_depthStencilStates.resize(DR_DEPTH_STENCIL_STATES::kCount);
  /*BLEND STATES*/
  ////Opaque
  DrBlendStateDesc blendDesc;
  blendDesc.blendEnable = false;
  {
    auto rs = m_device->createBlendState(blendDesc);
    m_blendSStates[DR_BLEND_STATES::kOpaque] = dr_gfx_unique(rs);
  }
  ////Additive
  blendDesc.blendEnable = true;;
  blendDesc.srcBlend =
    blendDesc.srcBlendAlpha = DR_BLEND::kBLEND_SRC_ALPHA;
  blendDesc.destBlend =
    blendDesc.destBlendAlpha = DR_BLEND::kBLEND_ONE;
  blendDesc.blendOp = DR_BLEND_OP::kBLEND_OP_ADD;
  blendDesc.blendOpAlpha = DR_BLEND_OP::kBLEND_OP_ADD;
  {
    auto rs = m_device->createBlendState(blendDesc);
    m_blendSStates[DR_BLEND_STATES::kAdditive] = dr_gfx_unique(rs);
  }
  ////AlphaBlend
  blendDesc.blendEnable = true;;
  blendDesc.srcBlend = DR_BLEND::kBLEND_SRC_ALPHA;
  blendDesc.srcBlendAlpha = DR_BLEND::kBLEND_ONE;
  blendDesc.destBlend = DR_BLEND::kBLEND_INV_SRC_ALPHA;
  blendDesc.destBlendAlpha = DR_BLEND::kBLEND_ZERO;
  blendDesc.blendOp = DR_BLEND_OP::kBLEND_OP_ADD;
  blendDesc.blendOpAlpha = DR_BLEND_OP::kBLEND_OP_ADD;
  {
    auto rs = m_device->createBlendState(blendDesc);
    m_blendSStates[DR_BLEND_STATES::kAlphaBlend] = dr_gfx_unique(rs);
  }

  /*DEPTH STATES*/
  DrDepthStencilStateDesc BlendDesc0;
  DrDepthStencilStateDesc BlendDesc1;
  DrDepthStencilStateDesc BlendDesc2;
  ////Read-Write
  BlendDesc0.depthFunc = DR_COMPARISON_FUNC::kLESS_EQUAL;
  {
    auto bd = m_device->createDepthStencilState(BlendDesc0);
    m_depthStencilStates[DR_DEPTH_STENCIL_STATES::kDepthRW] = dr_gfx_unique(bd);
  }
  //// DepthNone
  BlendDesc1.depthEnable = false;
  BlendDesc1.depthWriteMask = DR_DEPTH_WRITE_MASK::kMASK_ZERO;
  BlendDesc1.depthFunc = DR_COMPARISON_FUNC::kEQUAL;
  {
    auto bd = m_device->createDepthStencilState(BlendDesc1);
    m_depthStencilStates[DR_DEPTH_STENCIL_STATES::kNone] = dr_gfx_unique(bd);
  }
  //// DepthRead
  BlendDesc2.depthWriteMask = DR_DEPTH_WRITE_MASK::kMASK_ZERO;
  BlendDesc2.depthFunc = DR_COMPARISON_FUNC::kLESS_EQUAL;
  {
    auto bd = m_device->createDepthStencilState(BlendDesc2);
    m_depthStencilStates[DR_DEPTH_STENCIL_STATES::kDepthR] = dr_gfx_unique(bd);
  }
}

void
GraphicsAPI::destroy() {
}

void
GraphicsAPI::clear() {
  float rgba[4];
  rgba[0] = 0.25f;
  rgba[1] = 0.35f;
  rgba[2] = 0.45f;
  rgba[3] = 1.0f;
  m_deviceContext->clearRenderTargetView(m_swapChain->getBackBufferRT(), rgba);
  m_deviceContext->clearDepthStencilView(*m_depthStencilView,
                                         DR_DEPTH_STENCIL_CLEAR_TYPE::kClearDepth,
                                         1.0f,
                                         0);
}

void
GraphicsAPI::swapBuffers() {
  m_swapChain->swapBuffers();
}

void
GraphicsAPI::resizeBackBuffer(UInt32 w, UInt32 h) {

  m_swapChain->resize(*m_device, w,h);
  m_depthStencilView.reset();
  DrDepthStencilDesc depthTextureDesc;
  depthTextureDesc.bindFlags = DR_BIND_FLAGS::DEPTH_STENCIL | DR_BIND_FLAGS::SHADER_RESOURCE;
  depthTextureDesc.width = w;
  depthTextureDesc.height = h;
  depthTextureDesc.Format = DR_FORMAT::kD24_UNORM_S8_UINT;
  //depthTextureDesc.Format = DR_FORMAT::kD32_FLOAT;
  {
    auto ds = m_device->createDepthStencil(depthTextureDesc);
    m_depthStencilView = dr_gfx_unique(ds);
  }
  /////////////////////////////////////////////////////////////////////////
  m_swapChain->getBackBufferRT().set(*m_deviceContext, *m_depthStencilView);
  /////////////////////////////////////////////////////////////////////////
}

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
  return GraphicsDriver::API().m_swapChain->getBackBufferRT();
}

DepthStencil&
GraphicsAPI::getDepthStencil() {
  return *GraphicsDriver::API().m_depthStencilView;
}


RasterizerState&
GraphicsAPI::getRasterizerState() {
  return *GraphicsDriver::API().m_rasterizerState;
}

void* 
GraphicsAPI::getWindowHandle() {
  return GraphicsDriver::API().m_hwnd;
}

BlendState&
GraphicsAPI::getBlendState(DR_BLEND_STATES::E state) {
  return *GraphicsDriver::API().m_blendSStates[state];
}

DepthStencilState&
GraphicsAPI::getDepthStencilState(DR_DEPTH_STENCIL_STATES::E state) {
  return *GraphicsDriver::API().m_depthStencilStates[state];
}

}