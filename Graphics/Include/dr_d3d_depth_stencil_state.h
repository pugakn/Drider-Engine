#pragma once
#include "dr_depth_stencil_state.h"
#include <wrl.h>
#include <wrl/client.h>
class ID3D11DepthStencilState;
namespace driderSDK {
class D3DDepthStencilState : DepthStencilState {
public:
  DR_GRAPHICS_ERROR::E create(const Device& device, const DrDepthStencilDesc& desc) override;
  void set(const DeviceContext& deviceContext, UInt32 refValue)const override;
  void release() override;
  Microsoft::WRL::ComPtr<ID3D11DepthStencilState> APIState;

};
}
