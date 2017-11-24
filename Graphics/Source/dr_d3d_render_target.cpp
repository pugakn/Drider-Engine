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
                        const Texture& texture) {
  //D3D11_RENDER_TARGET_VIEW_DESC rtDesc;
  //ZeroMemory(&rtDesc, sizeof(D3D11_RENDER_TARGET_VIEW_DESC));
  //rtDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;//Hardcoded
  //rtDesc.Format = static_cast<DXGI_FORMAT>(texture.descriptor.Format);

    if (reinterpret_cast<const D3DDevice*>(&device)->
          D3D11Device->
            CreateRenderTargetView(reinterpret_cast<const D3DTexture*>(&texture)->
                                     APITexture,
                                   NULL,
                                   &RTV) != S_OK) {
      return DR_GRAPHICS_ERROR::CREATE_RESOURCE_VIEW_ERROR;
  }
  return DR_GRAPHICS_ERROR::ERROR_NONE;
}

void
D3DRenderTarget::set(const DeviceContext& deviceContext,
                     const DepthStencil& depthStencil) const {
  reinterpret_cast<const D3DDeviceContext*>(&deviceContext)->
    D3D11DeviceContext->
      OMSetRenderTargets(1,
                         &RTV,
                         reinterpret_cast<const D3DDepthStencil*>(&depthStencil)->
                           APIDepthView);
}

void
D3DRenderTarget::release() {
    RTV->Release();
    delete this;
}

}