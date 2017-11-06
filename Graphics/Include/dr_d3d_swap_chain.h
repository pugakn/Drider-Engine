#pragma once
#include "dr_swap_chain.h"

struct IDXGISwapChain;
struct ID3D11RenderTargetView;

namespace driderSDK {

class D3DSwapChain : public SwapChain {
 public:
  DR_GRAPHICS_ERROR::E
  create(const Device& device, const DrSwapChainDesc& desc) override;

  DR_GRAPHICS_ERROR::E
  getBackBuffer(Texture& texture) override;

  void
  release() override;

  void
  swapBuffers() override;

  IDXGISwapChain* APISwapchain;
  ID3D11RenderTargetView* APIRenderTargetView;  // View into the back buffer
};

}