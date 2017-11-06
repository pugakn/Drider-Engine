#pragma once
#include "dr_swap_chain.h"

struct IDXGISwapChain;
struct ID3D11RenderTargetView;

namespace driderSDK {

class D3DSwapChain : public SwapChain {
 public:
  /**
  * Create a swap chain
  *
  * @param device
  *   The device to create the swap chain
  *
  * @param desc
  *   Describes the swap chain params
  *
  * @return
  *   Return a DR_GRAPHICS_ERROR code, ERROR_NONE means all went well
  */
  DR_GRAPHICS_ERROR::E
  create(const Device& device, const DrSwapChainDesc& desc) override;

  /**
  * Get the back buffer
  *
  * @param out texture
  *   The texture to fill out
  *
  * @return
  *   Return a DR_GRAPHICS_ERROR code, ERROR_NONE means all went well
  */
  DR_GRAPHICS_ERROR::E
  getBackBuffer(Texture& texture) override;

  /**
  * Release the allocated memory
  */
  void
  release() override;

  /**
  * Swap back-front buffer
  */
  void
  swapBuffers() override;

  IDXGISwapChain* APISwapchain;
  ID3D11RenderTargetView* APIRenderTargetView;  // View into the back buffer
};

}