#pragma once
#include "dr_swap_chain.h"
#include "dr_graphics_prerequisites.h"
#include "dr_gfx_memory.h"

struct IDXGISwapChain;

namespace driderSDK {
  class D3DTexture;
  class RenderTarget;
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
  RenderTarget&
  getBackBufferRT() override;

  /**
  * TEST::release
  *
  * Release the allocated memory
  */
  void
  release() override;

  /**
  * Resize back buffer.
  * This should be called when the application window is resized.
  */
  void
  resize(const Device& device, UInt32 _w, UInt32 _h) override;


  /**
  * TEST::swapBuffers
  *
  * Swap back-front buffer
  */
  void
  swapBuffers() override;

private:
  IDXGISwapChain* APISwapchain;
  RenderTarget* m_backBufferView;
  D3DTexture* m_backBufferTexture;
};

}