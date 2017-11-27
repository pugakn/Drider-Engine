#pragma once
#include "dr_swap_chain.h"
#include "dr_graphics_prerequisites.h"

struct IDXGISwapChain;
struct ID3D11RenderTargetView;

namespace driderSDK {

class DR_GRAPHICS_EXPORT D3DSwapChain : public SwapChain
{
 public:
   void*
     getAPIObject() override;

   void**
     getAPIObjectReference() override;

  /**
  * TEST::create
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
  void
  create(const Device& device, const DrSwapChainDesc& desc) override;

  /**
  * TEST::getBackBuffer
  *
  * Get the back buffer
  *
  * @param out texture
  *   The texture to fill out
  *
  * @return
  *   Return a DR_GRAPHICS_ERROR code, ERROR_NONE means all went well
  */
  void
  getBackBuffer(Texture& texture) override;

  /**
  * TEST::release
  *
  * Release the allocated memory
  */
  void
  release() override;

  /**
  * TEST::swapBuffers
  *
  * Swap back-front buffer
  */
  void
  swapBuffers() override;

  IDXGISwapChain* APISwapchain;
  ID3D11RenderTargetView* APIRenderTargetView;  // View into the back buffer
};

}