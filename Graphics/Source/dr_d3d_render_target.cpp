#include "dr_d3d_render_target.h"
#include <d3d11.h>
#include <dxgi.h>
#include "dr_d3d_device.h"
#include "dr_d3d_device_context.h"
#include "dr_d3d_texture.h"
#include "dr_d3d_depth_stencil.h"

namespace driderSDK {
  void * D3DRenderTarget::getAPIObject()
  {
    return RTV;
  }
  void ** D3DRenderTarget::getAPIObjectReference()
  {
    return reinterpret_cast<void**>(&RTV);
  }
  void
D3DRenderTarget::create(const Device& device,
                        const Texture& texture) {
  reinterpret_cast<const D3DDevice*>(&device)->
    D3D11Device->
    CreateRenderTargetView(reinterpret_cast<const D3DTexture*>(&texture)->
      APITexture,
      NULL,
      &RTV);
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