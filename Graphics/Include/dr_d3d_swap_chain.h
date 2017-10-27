#pragma once
#include "dr_swap_chain.h"
#include <dr_prerequisites.h>
#include <wrl.h>
#include <wrl/client.h>

struct IDXGISwapChain;
struct ID3D11RenderTargetView;

namespace driderSDK {

class DR_API_EXPORT D3DSwapChain : public SwapChain
{
 public:
  /**
  * TEST::
  *
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
  * TEST::
  *
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
  * TEST::
  *
  * Release the allocated memory
  */
  void
  release() override;

  /**
  * TEST::
  *
  * Swap back-front buffer
  */
  void
  swapBuffers() override;

  Microsoft::WRL::ComPtr<IDXGISwapChain> APISwapchain;
  Microsoft::WRL::ComPtr<ID3D11RenderTargetView> APIRenderTargetView;  // View into the back buffer
};

}