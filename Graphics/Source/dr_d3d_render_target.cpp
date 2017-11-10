#include "dr_d3d_render_target.h"
#include <d3d11.h>
#include <dxgi.h>
#include "dr_d3d_device.h"
#include "dr_d3d_device_context.h"
#include "dr_d3d_texture.h"
#include "dr_d3d_depth_stencil.h"

namespace driderSDK {

DR_GRAPHICS_ERROR::E
D3DRenderTarget::create(const Device& device,
                        const DrTextureDesc& colorDesc,
                        UInt32 _numColorTextures) {
  numColorTextures = _numColorTextures;
  D3D11_RENDER_TARGET_VIEW_DESC rtDesc;
  ZeroMemory(&rtDesc, sizeof(D3D11_RENDER_TARGET_VIEW_DESC));
  rtDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;//Hardcoded
  rtDesc.Format = static_cast<DXGI_FORMAT>(colorDesc.Format);

  for (size_t i = 0; i < numColorTextures; ++i) {
    colorTextures.push_back(new D3DTexture);
    DR_GRAPHICS_ERROR::E DRErr;
    DRErr = colorTextures.back()->createEmpty(device, colorDesc);

    if (DRErr != DR_GRAPHICS_ERROR::ERROR_NONE) {
      return DRErr;
    }

    ID3D11RenderTargetView* RTV;
    if (reinterpret_cast<const D3DDevice*>(&device)->
          D3D11Device->
            CreateRenderTargetView(reinterpret_cast<D3DTexture*>(colorTextures.back())->
                                     APITexture,
                                   NULL,
                                   &RTV) != S_OK) {
      return DR_GRAPHICS_ERROR::CREATE_RESOURCE_VIEW_ERROR;
    }

    APIColorViews.push_back(RTV);
  }
  return DR_GRAPHICS_ERROR::ERROR_NONE;
}

void
D3DRenderTarget::set(const DeviceContext& deviceContext,
                     const DepthStencil& depthStencil) const {
  reinterpret_cast<const D3DDeviceContext*>(&deviceContext)->
    D3D11DeviceContext->
      OMSetRenderTargets(numColorTextures,
                         &APIColorViews.back(),
                         reinterpret_cast<const D3DDepthStencil*>(&depthStencil)->
                           APIDepthView);
}

void
D3DRenderTarget::release() {
  for (size_t i = 0; i < numColorTextures; ++i) {
    delete colorTextures[i];
    APIColorViews[i]->Release();
  }
}

}