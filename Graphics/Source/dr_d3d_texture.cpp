#include "dr_d3d_texture.h"
#include <d3d11.h>
#include <dxgi.h>
#include "dr_d3d_device.h"
#include "dr_d3d_device_context.h"

namespace driderSDK {
  void * D3DTexture::getAPIObject()
  {
    return APIView;
  }
  void ** D3DTexture::getAPIObjectReference()
  {
    return reinterpret_cast<void**>(&APIView);
  }
  void D3DTexture::createFromMemory(const Device& device,
                                    const DrTextureDesc& desc,
                                    const char* buffer) {
  const D3DDevice* apiDevice = reinterpret_cast<const D3DDevice*>(&device);
  m_descriptor = desc;
  UInt32 flags = 0;
  if (desc.genMipMaps) {
    flags |= D3D11_RESOURCE_MISC_GENERATE_MIPS;
  }
  D3D11_TEXTURE2D_DESC apiDesc = { 0 };
  apiDesc.Width = desc.width;
  apiDesc.Height = desc.height;
  apiDesc.ArraySize = 1;
  apiDesc.MipLevels = desc.mipLevels;
  apiDesc.Format = static_cast<DXGI_FORMAT>(desc.Format);
  apiDesc.SampleDesc.Count = 1;
  apiDesc.SampleDesc.Quality = 0;
  apiDesc.BindFlags = desc.bindFlags;
  apiDesc.MiscFlags = flags;
  apiDesc.CPUAccessFlags = 0;
  apiDesc.Usage = D3D11_USAGE_DEFAULT;

  D3D11_SUBRESOURCE_DATA initData{};
  initData.pSysMem = buffer;
  initData.SysMemPitch = desc.pitch;

  D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
  ZeroMemory(&srvDesc, sizeof(srvDesc));
  D3D11_SRV_DIMENSION dim;
  srvDesc.Format = apiDesc.Format;

  switch (desc.dimension)
  {
  case DR_DIMENSION::k1D:
    dim = D3D11_SRV_DIMENSION_TEXTURE1D;
    break;
  case DR_DIMENSION::k2D:
    dim = D3D11_SRV_DIMENSION_TEXTURE2D;
    break;
  case DR_DIMENSION::k3D:
    dim = D3D11_SRV_DIMENSION_TEXTURE3D;
    break;
  case DR_DIMENSION::kCUBE_MAP:
    dim = D3D10_SRV_DIMENSION_TEXTURECUBE;
    break;
  }
  srvDesc.ViewDimension = dim;
  srvDesc.Texture2D.MipLevels = desc.mipLevels; 

  apiDevice->
    D3D11Device->
    CreateTexture2D(&apiDesc,
      buffer != 0 ? &initData : 0,
      &APITexture);
  apiDevice->
    D3D11Device->
    CreateShaderResourceView(APITexture,
      &srvDesc,
      &APIView);
  
}

void
D3DTexture::createEmpty(const Device& device, const DrTextureDesc& desc){
  return createFromMemory(device, desc, 0);
}

void
D3DTexture::map(const DeviceContext& deviceContext, char* buffer) {
  D3D11_MAPPED_SUBRESOURCE mappedResource;
  reinterpret_cast<const D3DDeviceContext*>(&deviceContext)->
    D3D11DeviceContext->
    Map(APITexture,
      0,
      D3D11_MAP_WRITE_DISCARD,
      0,
      &mappedResource);

  buffer = static_cast<char*>(mappedResource.pData);
  
}

void
D3DTexture::set(const DeviceContext& deviceContext, UInt32 slot) const {
  reinterpret_cast<const D3DDeviceContext*>(&deviceContext)->
    D3D11DeviceContext->
      PSSetShaderResources(slot, 1, &APIView);
}

void
D3DTexture::release() {
  APITexture->Release();
  APIView->Release();
  delete this;
}

void
D3DTexture::unmap(const DeviceContext& deviceContext) {
  reinterpret_cast<const D3DDeviceContext*>(&deviceContext)->
    D3D11DeviceContext->
      Unmap(APITexture, 0);
}

void
D3DTexture::udpateFromMemory(const DeviceContext& deviceContext,
                             const char* buffer,
                             size_t bufferSize) {
  D3D11_SUBRESOURCE_DATA initData{};
  initData.pSysMem = buffer;
  initData.SysMemPitch = m_descriptor.pitch;
  reinterpret_cast<const D3DDeviceContext*>(&deviceContext)->
    D3D11DeviceContext->
      UpdateSubresource(APITexture, 0, 0, buffer, initData.SysMemPitch, 0);
}

void
D3DTexture::generateMipMaps(const DeviceContext& deviceContext) const {
  reinterpret_cast<const D3DDeviceContext*>(&deviceContext)->
    D3D11DeviceContext->
      GenerateMips(APIView);
}

}