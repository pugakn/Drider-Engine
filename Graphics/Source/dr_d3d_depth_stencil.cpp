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
                            const DrDepthStencilDesc& desc) {
  _textureOwner = true;
  m_descriptor = desc;
  DrTextureDesc tdesc;
  tdesc.bindFlags = desc.bindFlags;
  tdesc.width = desc.width;
  tdesc.height = desc.height;
  tdesc.mipLevels = 1;

  if (m_descriptor.Format == DR_FORMAT::kD32_FLOAT) {
    tdesc.Format = DR_FORMAT::kR32_TYPELESS;
  }
  else if (m_descriptor.Format == DR_FORMAT::kD16_UNORM) {
    tdesc.Format = DR_FORMAT::kR16_TYPELESS;
  }
  else if (m_descriptor.Format == DR_FORMAT::kD24_UNORM_S8_UINT) {
    tdesc.Format = DR_FORMAT::kR24G8_TYPELESS;
  }
  m_texture = device.createEmptyTexture(tdesc);


  D3D11_DEPTH_STENCIL_VIEW_DESC dDesc;
  ZeroMemory(&dDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
  dDesc.Format = static_cast<DXGI_FORMAT>(desc.Format);
  dDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;

  reinterpret_cast<const D3DDevice*>(&device)->
    D3D11Device->
    CreateDepthStencilView(reinterpret_cast<const D3DTexture*>(m_texture)->
      APITexture,
      &dDesc, &APIDepthView);
}

void D3DDepthStencil::create(const Device & device, const Texture & texture)
{
  _textureOwner = false;
  auto tDesc = texture.getDescriptor();
  m_descriptor.bindFlags = tDesc.bindFlags;
  m_descriptor.width = tDesc.width;
  m_descriptor.height = tDesc.height;
 

  if (tDesc.Format == DR_FORMAT::kR32_TYPELESS) {
    m_descriptor.Format = DR_FORMAT::kD32_FLOAT;
  }
  else if (tDesc.Format == DR_FORMAT::kR16_TYPELESS) {
    m_descriptor.Format = DR_FORMAT::kD16_UNORM;
  }
  else if (tDesc.Format == DR_FORMAT::kR24G8_TYPELESS) {
    m_descriptor.Format = DR_FORMAT::kD24_UNORM_S8_UINT;
  }
  m_texture = const_cast<Texture*>(&texture);


  D3D11_DEPTH_STENCIL_VIEW_DESC dDesc;
  ZeroMemory(&dDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
  dDesc.Format = static_cast<DXGI_FORMAT>(m_descriptor.Format);
  dDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;

  reinterpret_cast<const D3DDevice*>(&device)->
    D3D11Device->
    CreateDepthStencilView(reinterpret_cast<const D3DTexture*>(m_texture)->
      APITexture,
      &dDesc, &APIDepthView);
}

void
D3DDepthStencil::release() {
  APIDepthView->Release();
  if (_textureOwner)
    m_texture->release();
  delete this;
}

}