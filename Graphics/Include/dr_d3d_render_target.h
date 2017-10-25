#pragma once
#include "dr_render_target.h"
#include <wrl.h>
#include <wrl/client.h>
namespace driderSDK {
class DR_API_EXPORT D3DRenderTarget : public RenderTarget{
public:
  DR_GRAPHICS_ERROR::E create(const Device& device, const DrTextureDesc& colorDesc, const DrTextureDesc& depthDesc) override;
  void set(const DeviceContext& deviceContext) const override;
  void release() override;

  Microsoft::WRL::ComPtr<ID3D11RenderTargetView>  APIColorView; 
  Microsoft::WRL::ComPtr<ID3D11DepthStencilView>  APIDepthView;
};
}