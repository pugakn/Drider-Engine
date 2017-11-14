#pragma once
#include "dr_depth_stencil.h"

struct ID3D11DepthStencilView;

namespace driderSDK {

/**
* Base class for depth stencil
*
* Sample usage:
*	DepthStencil* = new D3DdepthStencil;
*/
class DR_GRAPHICS_EXPORT D3DDepthStencil : public DepthStencil
{
 public:
  /**
  * TEST::create
  *
  * Create a depth stencil on this object
  *
  * @param device
  *   The device to create the depth stencil
  *
  * @param desc
  *   The descriptor of the depth stencil
  *
  * @return
  *   Return a DR_GRAPHICS_ERROR code, ERROR_NONE means all went well
  */
  DR_GRAPHICS_ERROR::E
  create(const Device& device, const Texture& texture) override;

  //void
  //set(const DeviceContext& deviceContext, const DepthStencil& depthStencil) const = 0;

  /**
  * TEST::release
  *
  * Release the allocated memory
  */
  void
  release() override;

  ID3D11DepthStencilView* APIDepthView;
};

}