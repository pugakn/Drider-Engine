#pragma once
#include "dr_rasterizer_state.h"
#include <wrl.h>
#include <wrl/client.h>
class ID3D11RasterizerState;
namespace driderSDK {
class DR_API_EXPORT D3DRasterizerState : public RasterizerState
{
public:
  DR_GRAPHICS_ERROR::E create(const Device& device,const DrRasterizerDesc& desc);
  void set(const DeviceContext& deviceContext);
  void release() override;
  Microsoft::WRL::ComPtr<ID3D11RasterizerState> APIState;
};
}