#pragma once
#include "dr_render_target.h"
#include <wrl.h>
#include <wrl/client.h>
class ID3D11RenderTargetView;
namespace driderSDK {
class DepthStencil;
class DR_API_EXPORT D3DRenderTarget : public RenderTarget{
public:
  DR_GRAPHICS_ERROR::E create(const Device& device, const DrTextureDesc& colorDesc) override;
  void set(const DeviceContext& deviceContext, const DepthStencil& depthStencil) const override;
  void release() override;
  Microsoft::WRL::ComPtr<ID3D11RenderTargetView>  APIColorView; 
};
}