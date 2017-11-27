#include "dr_d3d_depth_stencil.h"
#include <d3d11.h>
#include <dxgi.h>
#include "dr_d3d_device.h"
#include "dr_d3d_device_context.h"
#include "dr_d3d_texture.h"

namespace driderSDK {
  void * D3DDepthStencil::getAPIObject()
  {
    return APIDepthView;
  }
  void ** D3DDepthStencil::getAPIObjectReference()
  {
    return reinterpret_cast<void**>(&APIDepthView);
  }
  void D3DDepthStencil::create(const Device& device,
                             const Texture& texture) {
  D3D11_DEPTH_STENCIL_VIEW_DESC dDesc;
  ZeroMemory(&dDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
  dDesc.Format = static_cast<DXGI_FORMAT>(texture.m_descriptor.Format);
  dDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;

  reinterpret_cast<const D3DDevice*>(&device)->
    D3D11Device->
    CreateDepthStencilView(reinterpret_cast<const D3DTexture*>(&texture)->
      APITexture,
      &dDesc, &APIDepthView);
}

void
D3DDepthStencil::release() {
  APIDepthView->Release();
  delete this;
}

}