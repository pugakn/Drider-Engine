#include "dr_d3d_render_target.h"
#include <d3d11.h>
#include <dxgi.h>
#include "dr_d3d_device.h"
#include "dr_d3d_device_context.h"
#include "dr_d3d_texture.h"
#include "dr_d3d_depth_stencil.h"
namespace driderSDK {
  DR_GRAPHICS_ERROR::E D3DRenderTarget::create(const Device & device, const DrTextureDesc & colorDesc)
  {
    colorTexture = std::make_unique<D3DTexture>();

    DR_GRAPHICS_ERROR::E DRErr;
    DRErr = colorTexture->createEmpty(device, colorDesc);
    if (DRErr != DR_GRAPHICS_ERROR::ERROR_NONE)
    {
      return DRErr;
    }

    D3D11_RENDER_TARGET_VIEW_DESC rtDesc;
    ZeroMemory(&rtDesc, sizeof(D3D11_RENDER_TARGET_VIEW_DESC));
    rtDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;//Hardcoded
    rtDesc.Format = static_cast<DXGI_FORMAT>(colorDesc.Format);
    if (static_cast<const D3DDevice*>(&device)->D3D11Device->CreateRenderTargetView(static_cast<D3DTexture*>(colorTexture.get())->APITexture.Get(), NULL, &APIColorView) != S_OK)
    {
      return DR_GRAPHICS_ERROR::CREATE_RESOURCE_VIEW_ERROR;
    }
    
    return DR_GRAPHICS_ERROR::ERROR_NONE;
  }
  void D3DRenderTarget::set(const DeviceContext & deviceContext, const DepthStencil& depthStencil) const
  {
    static_cast<const D3DDeviceContext*>(&deviceContext)->D3D11DeviceContext->OMSetRenderTargets(1,APIColorView.GetAddressOf(), static_cast<const D3DDepthStencil*>(&depthStencil)->APIDepthView.Get());
  }
  void D3DRenderTarget::release()
  {
    colorTexture.release();
    APIColorView.Reset();
  }
}