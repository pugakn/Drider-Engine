#include "dr_d3d_graphics_api.h"
#include "dr_d3d_device.h"
#include "dr_d3d_device_context.h"
#include "dr_d3d_swap_chain.h"
#include "dr_d3d_depth_stencil.h"
#include "dr_d3d_render_target.h"
#include "dr_d3d_texture.h"
#include "dr_d3d_rasterizer_state.h"
#include <dxgi.h>
#include <d3d11.h>
#include <DirectXMath.h>
namespace driderSDK {
void D3DGraphicsAPI::init(UInt32 w, UInt32 h, void* hwnd)
{
  m_hwnd = (size_t)hwnd;
  device = new D3DDevice;
  deviceContext = new D3DDeviceContext;
  backBufferTexture = new D3DTexture;
  device->createDeviceAndDeviceContext(*deviceContext);

  //	Descriptor of the Swap Chain
  DrSwapChainDesc swapDesc;
  swapDesc.bufferCount = 1;
  swapDesc.windowed = true;
  swapDesc.windowHandler = hwnd;
  swapDesc.height = h;
  swapDesc.width = w;

  swapChain =  device->createSwapChain(swapDesc);

  DrTextureDesc depthTextureDesc;
  depthTextureDesc.bindFlags = DR_BIND_FLAGS::DEPTH_STENCIL;
  depthTextureDesc.width = w;
  depthTextureDesc.height = h;
  depthTextureDesc.mipLevels = 1;
  depthTextureDesc.Format = DR_FORMAT::kDrFormat_D24_UNORM_S8_UINT;
  depthTexture =  device->createEmptyTexture(depthTextureDesc);
  depthStencilView =  device->createDepthStencil(*depthTexture);


  swapChain->getBackBuffer(*backBufferTexture);
  backBufferView =  device->createRenderTarget(*backBufferTexture);
  delete backBufferTexture;
  backBufferView->set(*deviceContext, *depthStencilView);

  D3D11_VIEWPORT viewport = { 0 };
  viewport.TopLeftX = 0;
  viewport.TopLeftY = 0;
  viewport.Width = static_cast<float>(w);
  viewport.Height = static_cast<float>(h);
  viewport.MinDepth = 0;
  viewport.MaxDepth = 1;

  reinterpret_cast<D3DDeviceContext*>(deviceContext)->D3D11DeviceContext->RSSetViewports(1, &viewport);

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
  rasterizeState = device->createRasteizerState(rasterizerStateDesc);
  rasterizeState->set(*deviceContext);
}
void D3DGraphicsAPI::destroy()
{
  device->release();
  deviceContext->release();
  swapChain->release();
  backBufferView->release();
  depthStencilView->release();
  depthTexture->release();
  rasterizeState->release();
}
void D3DGraphicsAPI::clear()
{
  float rgba[4];
  rgba[0] = 0.25f;
  rgba[1] = 0.35f;
  rgba[2] = 0.45f;
  rgba[3] = 1.0f;
  deviceContext->clearRenderTargetView(*backBufferView, rgba);
  deviceContext->clearDepthStencilView(*depthStencilView, DR_DEPTH_STENCIL_CLEAR_TYPE::kClearDepth, 1.0f, 0);
}
void D3DGraphicsAPI::swapBuffers()
{
  swapChain->swapBuffers();
}
}