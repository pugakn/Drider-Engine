#include "dr_d3d_render_target.h"
#include <d3d11.h>
#include <dxgi.h>
#include "dr_d3d_device.h"
#include "dr_d3d_device_context.h"
#include "dr_d3d_texture.h"
namespace driderSDK {
  DR_GRAPHICS_ERROR::E D3DRenderTarget::create(const Device & device, const DrTextureDesc & colorDesc, const DrTextureDesc & depthDesc)
  {
    colorTexture = std::make_unique<D3DTexture>();
    depthTexture = std::make_unique<D3DTexture>();

    DR_GRAPHICS_ERROR::E DRErr;
    DRErr = colorTexture->createEmpty(device, colorDesc);
    if (DRErr != DR_GRAPHICS_ERROR::ERROR_NONE)
    {
      return DRErr;
    }
    DRErr = depthTexture->createEmpty(device, depthDesc);
    if (DRErr != DR_GRAPHICS_ERROR::ERROR_NONE)
    {
      return DRErr;
    }	D3D11_DEPTH_STENCIL_VIEW_DESC dsvd;
    ZeroMemory(&dsvd, sizeof(dsvd));

    dsvd.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    dsvd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;

    if (static_cast<const D3DDevice*>(&device)->D3D11Device->CreateDepthStencilView(static_cast<D3DTexture*>(depthTexture.get())->APITexture.Get(), &dsvd, &APIDepthView) != S_OK)
    {
      return DR_GRAPHICS_ERROR::CREATE_RESOURCE_VIEW_ERROR;
    }
    if (static_cast<const D3DDevice*>(&device)->D3D11Device->CreateRenderTargetView(static_cast<D3DTexture*>(depthTexture.get())->APITexture.Get(), NULL, &APIColorView) != S_OK)
    {
      return DR_GRAPHICS_ERROR::CREATE_RESOURCE_VIEW_ERROR;
    }
    
    return DR_GRAPHICS_ERROR::ERROR_NONE;
  }
  void D3DRenderTarget::set(const DeviceContext & deviceContext) const
  {
    static_cast<const D3DDeviceContext*>(&deviceContext)->D3D11DeviceContext->OMSetRenderTargets(1, APIColorView.GetAddressOf(), APIDepthView.Get());
  }
  void D3DRenderTarget::release()
  {
    colorTexture.release();
    depthTexture.release();
    APIColorView.Reset();
    APIDepthView.Reset();
  }
}