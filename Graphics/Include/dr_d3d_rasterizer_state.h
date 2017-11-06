#pragma once
#include "dr_rasterizer_state.h"

struct ID3D11RasterizerState;

namespace driderSDK {

class DR_API_EXPORT D3DRasterizerState : public RasterizerState
{
 public:
  DR_GRAPHICS_ERROR::E
  create(const Device& device, const DrRasterizerDesc& desc) override;

  void
  set(const DeviceContext& deviceContext) const override;

  void
  release() override;

  ID3D11RasterizerState* APIState;
};

}