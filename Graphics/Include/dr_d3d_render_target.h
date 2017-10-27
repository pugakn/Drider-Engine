#pragma once
#include "dr_render_target.h"
#include <wrl.h>
#include <wrl/client.h>
#include <vector>
class ID3D11RenderTargetView;

namespace driderSDK {

class DepthStencil;

/**
* Base class for render target
*
* Sample usage:
*	RenderTarget* = new D3DRenderTarget;
*/
class DR_API_EXPORT D3DRenderTarget : public RenderTarget
{
 public:
  /**
  * TEST::create
  *
  * Create a render target on this object
  *
  * @param device
  *   The device to create the depth stencil
  *
  * @param desc
  *   The descriptor of the depth stencil
  *
  * @param _numColorTextures
  *   The number of textures to create for the render target
  *
  * @return
  *   Return a DR_GRAPHICS_ERROR code, ERROR_NONE means all went well
  */
  DR_GRAPHICS_ERROR::E
  create(const Device& device,
         const DrTextureDesc& colorDesc,
         UInt32 numRT) override;

  /**
  * TEST::set
  *
  * Set the render target and depth stencil
  *
  * @param deviceContext
  *   The device context to set the shader
  *
  * @param depthStencil
  *   The depth stencil to set with the render target
  *
  */
  void
  set(const DeviceContext& deviceContext,
      const DepthStencil& depthStencil) const override;

  /**
  * TEST::release
  *
  * Release the allocated memory
  */
  void
  release() override;

  std::vector<Microsoft::WRL::ComPtr<ID3D11RenderTargetView>> APIColorViews; 
};

}