#include "dr_d3d_graphics_api.h"
#include "dr_d3d_device.h"
#include "dr_d3d_device_context.h"
#include "dr_d3d_swap_chain.h"
#include "dr_d3d_depth_stencil.h"
#include "dr_d3d_render_target.h"
#include "dr_d3d_texture.h"
#include "dr_d3d_rasterizer_state.h"
namespace driderSDK {
void D3DGraphicsAPI::init(UInt32 w, UInt32 h, void* hwnd)
{
  device = new D3DDevice;
  device->createDeviceAndDeviceContext(*deviceContext);
  backBufferTexture = new D3DTexture;

  //	Descriptor of the Swap Chain
  DrSwapChainDesc swapDesc;
  swapDesc.bufferCount = 1;
  swapDesc.windowed = true;
  swapDesc.windowHandler = hwnd;
  swapDesc.height = h;
  swapDesc.width = w;

  device->createSwapChain(swapDesc, swapChain);

  DrTextureDesc depthTextureDesc;
  depthTextureDesc.bindFlags = DR_BIND_FLAGS::DEPTH_STENCIL;
  depthTextureDesc.width = w;
  depthTextureDesc.height = h;
  depthTextureDesc.mipLevels = 1;
  depthTextureDesc.Format = DR_FORMAT::kDrFormat_D24_UNORM_S8_UINT;
  device->createEmptyTexture(depthTextureDesc,depthTexture);
  device->createDepthStencil(*depthTexture,depthStencilView);


  swapChain->getBackBuffer(*backBufferTexture);
  device->createRenderTarget(*backBufferTexture,backBufferView);
  delete backBufferTexture;

  backBufferView->set(*deviceContext, *depthStencilView);
  DrRasterizerDesc rasterizerStateDesc;
  rasterizerStateDesc.fillMode = DR_FILL_MODE::kSolid;
  rasterizerStateDesc.cullMode = DR_CULL_MODE::kBack;
  rasterizerStateDesc.frontCounterClockwise = true;
  rasterizerStateDesc.depthBias = 0;
  rasterizerStateDesc.depthBiasClamp = 0.0f;
  rasterizerStateDesc.slopeScaledDepthBias = 0.0f;
  rasterizerStateDesc.depthClipEnable = true;
  rasterizerStateDesc.scissorEnable = false;
  rasterizerStateDesc.multisampleEnable = false;
  rasterizerStateDesc.antialiasedLineEnable = false;
  device->createRasteizerState(rasterizerStateDesc,rasterizeState);
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
}