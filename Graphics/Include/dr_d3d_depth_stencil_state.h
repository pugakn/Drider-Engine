#pragma once
#include "dr_depth_stencil_state.h"
#include "dr_graphics_prerequisites.h"

class ID3D11DepthStencilState;

namespace driderSDK {

class DR_GRAPHICS_EXPORT D3DDepthStencilState : public DepthStencilState
{
 public:
   void*
   getAPIObject() override;

   void**
   getAPIObjectReference() override;

  /**
  * TEST::
  *
  * Create a detph stencil state that describes the behavior
  * of the deth stencil
  *
  * @param device
  *   The device to create the depth stencil state
  *
  * @param desc
  *   The descriptor of the depth stencil state
  *
  * @return
  *   Return a DR_GRAPHICS_ERROR code, ERROR_NONE means all went well
  */
  void
  create(const Device& device, const DrDepthStencilDesc& desc) override;

  /**
  * TEST::
  *
  * Set the depth stencil state
  *
  * @param deviceContext
  *   The device context set the state
  *
  * @param refValue
  *   Ther reference value to perform against when doing a
  *   depth-stencil test
  */
  void
  set(const DeviceContext& deviceContext, UInt32 refValue) const override;

  /**
  * TEST::
  *
  * Release the allocated memory
  */
  void
  release() override;

  ID3D11DepthStencilState* APIState;
};

}