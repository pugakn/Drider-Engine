#include "dr_d3d_graphics_api.h"
#include "dr_d3d_device.h"
#include "dr_d3d_device_context.h"
#include "dr_d3d_swap_chain.h"
#include "dr_d3d_depth_stencil.h"
#include "dr_d3d_render_target.h"
#include "dr_d3d_texture.h"
#include "dr_d3d_rasterizer_state.h"
#include "dr_d3d_blend_state.h"
#include <dxgi.h>
#include <d3d11.h>
#include <DirectXMath.h>
namespace driderSDK {

void D3DGraphicsAPI::init(UInt32 w, UInt32 h, void* hwnd) {

  m_hwnd = hwnd;

  m_device = dr_unique_custom(new D3DDevice, &dr_gfx_deleter<Device>);

  m_deviceContext = dr_unique_custom(new D3DDeviceContext, 
                                     &dr_gfx_deleter<DeviceContext>);

  /* WHUT */
  auto backBufferTexture = dr_make_unique<D3DTexture>();

  m_device->createDeviceAndDeviceContext(*m_deviceContext);

  //	Descriptor of the Swap Chain
  DrSwapChainDesc swapDesc;
  swapDesc.bufferCount = 1;
  swapDesc.windowed = true;
  swapDesc.windowHandler = hwnd;
  swapDesc.height = h;
  swapDesc.width = w;

  m_swapChain = dr_unique_custom(m_device->createSwapChain(swapDesc),
                                 &dr_gfx_deleter<SwapChain>);

  DrTextureDesc depthTextureDesc;
  depthTextureDesc.bindFlags = DR_BIND_FLAGS::DEPTH_STENCIL;
  depthTextureDesc.width = w;
  depthTextureDesc.height = h;
  depthTextureDesc.mipLevels = 1;
  depthTextureDesc.Format = DR_FORMAT::kD24_UNORM_S8_UINT;

  {
    auto dt = m_device->createEmptyTexture(depthTextureDesc);
    m_depthTexture = dr_unique_custom(dt, &dr_gfx_deleter<Texture>);
  }  

  {
    auto ds = m_device->createDepthStencil(*m_depthTexture);
    m_depthStencilView = dr_unique_custom(ds, &dr_gfx_deleter<DepthStencil>);
  }  

  m_swapChain->getBackBuffer(*backBufferTexture);

  {
    std::vector<Texture*> texturesVec;
    texturesVec.push_back(backBufferTexture.get());
    auto bbRT = m_device->createRenderTarget(texturesVec);

    m_backBufferView = dr_unique_custom(bbRT, &dr_gfx_deleter<RenderTarget>);
  }
  
  
  m_backBufferView->set(*m_deviceContext, *m_depthStencilView);

  D3D11_VIEWPORT viewport = { 0 };
  viewport.TopLeftX = 0;
  viewport.TopLeftY = 0;
  viewport.Width = static_cast<float>(w);
  viewport.Height = static_cast<float>(h);
  viewport.MinDepth = 0;
  viewport.MaxDepth = 1;

  auto d3dDC = reinterpret_cast<D3DDeviceContext*>(m_deviceContext.get());

  d3dDC->D3D11DeviceContext->RSSetViewports(1, &viewport);

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

    m_rasterizerState = dr_unique_custom(rs, &dr_gfx_deleter<RasterizerState>);
  }

  m_rasterizerState->set(*m_deviceContext);

  /*BLEND STATES*/
  ////Opaque
  DrBlendStateDesc blendDesc;
  {
    auto rs = m_device->createBlendState(blendDesc);
    m_blendSTOpaque = dr_unique_custom(rs, &dr_gfx_deleter<BlendState>);
  }
  ////Additive
  blendDesc.blendEnable = true;;
  blendDesc.srcBlend =
  blendDesc.srcBlendAlpha = DR_BLEND::kBLEND_SRC_ALPHA;
  blendDesc.destBlend =
  blendDesc.destBlendAlpha = DR_BLEND::kBLEND_ONE;
  {
    auto rs = m_device->createBlendState(blendDesc);
    m_blendSTAdditive = dr_unique_custom(rs, &dr_gfx_deleter<BlendState>);
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
    m_blendSTAlphaBlend = dr_unique_custom(rs, &dr_gfx_deleter<BlendState>);
  }


}

void D3DGraphicsAPI::destroy() {}

void D3DGraphicsAPI::clear() {
  float rgba[4];
  rgba[0] = 0.25f;
  rgba[1] = 0.35f;
  rgba[2] = 0.45f;
  rgba[3] = 1.0f;
  m_deviceContext->clearRenderTargetView(*m_backBufferView, rgba);
  m_deviceContext->clearDepthStencilView(*m_depthStencilView, 
                                         DR_DEPTH_STENCIL_CLEAR_TYPE::kClearDepth, 
                                         1.0f, 0);
}

void D3DGraphicsAPI::swapBuffers() {
  m_swapChain->swapBuffers();
}

}