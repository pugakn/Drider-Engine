#include "dr_d3d_texture.h"
#include <d3d11.h>
#include <dxgi.h>
#include "dr_d3d_device.h"
#include "dr_d3d_device_context.h"

namespace driderSDK {

DR_GRAPHICS_ERROR::E D3DTexture::createFromMemory(const Device& device,
                                                  const DrTextureDesc& desc,
                                                  const char* buffer) {
  descriptor = desc;
  D3D11_TEXTURE2D_DESC apiDesc = { 0 };
  apiDesc.Width = desc.width;
  apiDesc.Height = desc.height;
  apiDesc.ArraySize = 1;
  apiDesc.Format = static_cast<DXGI_FORMAT>(desc.Format);
  apiDesc.SampleDesc.Count = 1;
  apiDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET | D3D11_BIND_DEPTH_STENCIL; //Hardcoded
  apiDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;//Hardcoded

  D3D11_SUBRESOURCE_DATA initData{};
  initData.pSysMem = buffer;
  initData.SysMemPitch = desc.pitch;

  D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc{};
  srvDesc.Format = apiDesc.Format;
  srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D; //Hardcoded
  srvDesc.Texture2D.MipLevels = -1;                      //Hardcoded
  if (static_cast<const D3DDevice*>(&device)->
        D3D11Device->
          CreateTexture2D(&apiDesc,
                          buffer != 0 ? &initData : nullptr,
                          APITexture.GetAddressOf()) != S_OK) {
    return DR_GRAPHICS_ERROR::CREATE_TEXTURE_ERROR;
  }
  if (static_cast<const D3DDevice*>(&device)->
        D3D11Device->
          CreateShaderResourceView(APITexture.Get(),
                                   &srvDesc,
                                   APIView.GetAddressOf()) != S_OK) {
    return DR_GRAPHICS_ERROR::CREATE_RESOURCE_VIEW_ERROR;
  }
  return DR_GRAPHICS_ERROR::ERROR_NONE;
}

DR_GRAPHICS_ERROR::E
D3DTexture::createEmpty(const Device& device, const DrTextureDesc& desc){
  return createFromMemory(device, desc, 0);
}

DR_GRAPHICS_ERROR::E
D3DTexture::map(const DeviceContext& deviceContext, char* buffer) {
  D3D11_MAPPED_SUBRESOURCE mappedResource;

  //Hardcoded
  if (static_cast<const D3DDeviceContext*>(&deviceContext)->
        D3D11DeviceContext->
          Map(APITexture.Get(),
              0,
              D3D11_MAP_WRITE_DISCARD,
              0,
              &mappedResource) != S_OK) {
    return DR_GRAPHICS_ERROR::MAP_RESOURCE_ERROR;
  }

  buffer = static_cast<char*>(mappedResource.pData);
  return DR_GRAPHICS_ERROR::ERROR_NONE;
}

void
D3DTexture::set(const DeviceContext& deviceContext, UInt32 slot) const {
  static_cast<const D3DDeviceContext*>(&deviceContext)->
    D3D11DeviceContext->
      PSSetShaderResources(slot, 1, APIView.GetAddressOf());
}

void
D3DTexture::release() {
  APITexture.Reset();
  APIView.Reset();
}

void
D3DTexture::unmap(const DeviceContext& deviceContext) {
  static_cast<const D3DDeviceContext*>(&deviceContext)->
    D3D11DeviceContext->
      Unmap(APITexture.Get(), 0);
}

void
D3DTexture::udpateFromMemory(const DeviceContext& deviceContext,
                             const char* buffer,
                             size_t bufferSize) {
  D3D11_SUBRESOURCE_DATA initData{};
  initData.pSysMem = buffer;
  initData.SysMemPitch = descriptor.pitch;
  static_cast<const D3DDeviceContext*>(&deviceContext)->
    D3D11DeviceContext->
      UpdateSubresource(APITexture.Get(), 0, 0, buffer, initData.SysMemPitch, 0);
}

void
D3DTexture::generateMipMaps(const DeviceContext& deviceContext) const {
  static_cast<const D3DDeviceContext*>(&deviceContext)->
    D3D11DeviceContext->
      GenerateMips(APIView.Get());
}

}