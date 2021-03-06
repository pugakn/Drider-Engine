#pragma once
#include "dr_rasterizer_state.h"

struct ID3D11RasterizerState;

namespace driderSDK {

class DR_GRAPHICS_EXPORT D3DRasterizerState : public RasterizerState
{
 public:
   void*
     getAPIObject() override;

   void**
     getAPIObjectReference() override;

  /**
  * TEST::create
  *
  * Create a sample state
  *
  * @param device
  *   The device to create the state
  *
  * @param desc
  *   Describes the state params
  *
  * @return
  *   Return a DR_GRAPHICS_ERROR code, ERROR_NONE means all went well
  */
  void
  create(const Device& device, const DrRasterizerDesc& desc) override;

  /**
  * TEST::set
  *
  * Set the state
  *
  * @param deviceContext
  *   The device context to set the state
  *
  * @param typeFlag
  *   Specifyes the shaders to bind with a bitflag
  */
  void
  set(const DeviceContext& deviceContext) const override;

  /**
  * TEST::release
  *
  * Release the allocated memory
  */
  void
  release() override;

  ID3D11RasterizerState* APIState;
};

}