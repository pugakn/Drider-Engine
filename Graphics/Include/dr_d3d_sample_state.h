#pragma once
#include "dr_sample_state.h"

struct ID3D11SamplerState;

namespace driderSDK {

class DR_API_EXPORT D3D11SamplerState : public SamplerState
{
 public:
  DR_GRAPHICS_ERROR::E
  create(const Device& device, const DrSampleDesc& desc) override;

  void
  set(const DeviceContext& deviceContext, DR_SHADER_TYPE_FLAG::E typeFlag) const override;

  void
  release() override;

  ID3D11SamplerState* APIState;
};

}