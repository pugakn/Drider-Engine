#pragma once
#include "dr_depth_stencil_state.h"
#include <wrl.h>
#include <wrl/client.h>

class Shader; 
class ID3D11DepthStencilState;

namespace driderSDK {

class D3DDepthStencilState : public DepthStencilState
{
 public:
  /**
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
  DR_GRAPHICS_ERROR::E
  create(const Device& device, const DrDepthStencilDesc& desc) override;

  /**
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
  * Release the allocated memory
  */
  void
  release() override;

  Microsoft::WRL::ComPtr<ID3D11DepthStencilState> APIState;
};

}