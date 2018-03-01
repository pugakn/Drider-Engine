#pragma once
#include "dr_render_target.h"
#include <vector>

struct ID3D11RenderTargetView;

namespace driderSDK {

class DepthStencil;

/**
* Base class for render target
*
* Sample usage:
*	RenderTarget* = new D3DRenderTarget;
*/
class DR_GRAPHICS_EXPORT D3DRenderTarget : public RenderTarget
{
 public:
   //void*
   //getAPIObject() override;

   //void**
   //getAPIObjectReference() override;
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
  void
  create(const Device& device, const std::vector<Texture*>& textures) override;

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

  std::vector<ID3D11RenderTargetView*> RTVs;
};

}