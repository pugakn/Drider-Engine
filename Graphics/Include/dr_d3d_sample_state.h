#pragma once
#include "dr_sample_state.h"
#include <wrl.h>
#include <wrl/client.h>

struct ID3D11SamplerState;

namespace driderSDK {

class DR_GRAPHICS_EXPORT D3D11SamplerState : public SamplerState
{
 public:
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
  DR_GRAPHICS_ERROR::E
  create(const Device& device, const DrSampleDesc& desc) override;

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
  set(const DeviceContext& deviceContext,
      DR_SHADER_TYPE_FLAG::E typeFlag) const override;

  /**
  * TEST::release
  *
  * Release the allocated memory
  */
  void
  release() override;

  Microsoft::WRL::ComPtr<ID3D11SamplerState> APIState;
};

}