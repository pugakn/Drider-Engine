#pragma once
#include "dr_depth_stencil.h"
#include <wrl.h>
#include <wrl/client.h>
struct ID3D11DepthStencilView;
namespace driderSDK {
class DR_API_EXPORT D3DDepthStencil : public DepthStencil {
public:
  DR_GRAPHICS_ERROR::E create(const Device& device, const DrTextureDesc& desc) override;
  //void set(const DeviceContext& deviceContext, const DepthStencil& depthStencil) const = 0;
  void release() override;
  Microsoft::WRL::ComPtr<ID3D11DepthStencilView> APIDepthView;
};
}