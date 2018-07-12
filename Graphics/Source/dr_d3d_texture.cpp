#include "dr_d3d_texture.h"

#include <d3d11.h>
#include <dxgi.h>
#include "dr_graphics_prerequisites.h"
#include <dr_math.h>
#include "dr_d3d_device.h"
#include "dr_d3d_device_context.h"
#include "dr_gfx_memory.h"


namespace driderSDK {

void*
D3DTexture::getAPIObject() {
  return APIView;
}

void**
D3DTexture::getAPIObjectReference() {
  DR_ASSERT(m_descriptor.bindFlags != DR_BIND_FLAGS::DEPTH_STENCIL);
  return reinterpret_cast<void**>(&APIView);
}

void
D3DTexture::createFromMemory(const Device& device,
                             const DrTextureDesc& desc,
                             const char* buffer) {
  D3D11_TEXTURE2D_DESC apiDesc = { 0 };
  const D3DDevice* apiDevice = reinterpret_cast<const D3DDevice*>(&device);
  m_descriptor = desc;
  UInt32 flags = 0;
  m_arraySize = 1;
  if (desc.dimension == DR_DIMENSION::kCUBE_MAP) {
    m_arraySize = 6;
    flags = D3D11_RESOURCE_MISC_TEXTURECUBE;
  }


  apiDesc.Width = desc.width;
  apiDesc.Height = desc.height;
  apiDesc.ArraySize = m_arraySize;
  apiDesc.MipLevels = desc.mipLevels;
  apiDesc.Format = static_cast<DXGI_FORMAT>(desc.Format);
  apiDesc.SampleDesc.Count = 1;
  apiDesc.SampleDesc.Quality = 0;
  apiDesc.BindFlags = desc.bindFlags;
  apiDesc.MiscFlags = flags;
  if (desc.CPUAccessFlags & DR_CPU_ACCESS_FLAG::drRead)
    apiDesc.CPUAccessFlags = desc.CPUAccessFlags ^ DR_CPU_ACCESS_FLAG::drRead;
  apiDesc.Usage = static_cast<D3D11_USAGE>(desc.Usage);//D3D11_USAGE_STAGING;//D3D11_USAGE_DEFAULT;

  if (desc.genMipMaps && desc.Format != DR_FORMAT::kBC3_UNORM) {
    apiDesc.MiscFlags |= D3D11_RESOURCE_MISC_GENERATE_MIPS;
    apiDesc.BindFlags |= DR_BIND_FLAGS::RENDER_TARGET;
  }

  D3D11_SUBRESOURCE_DATA initData[6];
  Int32 bufferSize = desc.pitch * desc.height;
  char *pHead = const_cast<char*>(buffer);
  for (UInt32 i = 0; i < m_arraySize; ++i) {
    initData[i].pSysMem = pHead;
    initData[i].SysMemPitch = desc.pitch;
    pHead += bufferSize;
  }
  

  D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
  ZeroMemory(&srvDesc, sizeof(srvDesc));
  srvDesc.Format = apiDesc.Format;
  D3D11_SRV_DIMENSION dim;
  dim = D3D11_SRV_DIMENSION_UNKNOWN;
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
  default:
    break;
  }
  srvDesc.ViewDimension = dim;
  if (desc.bindFlags & DR_BIND_FLAGS::DEPTH_STENCIL) {
    srvDesc.Texture2D.MipLevels = 1;
    if (apiDesc.Format == DXGI_FORMAT_R32_TYPELESS) {
      srvDesc.Format = DXGI_FORMAT_R32_FLOAT;
    }
    else if (apiDesc.Format == DXGI_FORMAT_R16_TYPELESS) {
      srvDesc.Format = DXGI_FORMAT_R16_FLOAT;
    }
    else if (apiDesc.Format == DXGI_FORMAT_R24G8_TYPELESS) {
      srvDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
    }
  }
  else {
    srvDesc.Texture2D.MipLevels = Math::MAX_UINT32;
  }



  HRESULT hr = apiDevice->
    D3D11Device->
    CreateTexture2D(&apiDesc,
      buffer ? initData : 0,
      &APITexture);
  
  apiDevice->
    D3D11Device->
    CreateShaderResourceView(APITexture,
      &srvDesc,
      &APIView);

  if (desc.bindFlags & DR_BIND_FLAGS::UNORDERED_ACCESS) {
    D3D11_UAV_DIMENSION dimUAV;
    D3D11_UNORDERED_ACCESS_VIEW_DESC viewDescUAV;
    ZeroMemory(&viewDescUAV, sizeof(viewDescUAV));
    viewDescUAV.Format = apiDesc.Format; //DXGI_FORMAT_R32G32B32A32_FLOAT;
    switch (desc.dimension)
    {
    case DR_DIMENSION::k1D:
      dimUAV = D3D11_UAV_DIMENSION_TEXTURE1D; 
      break;
    case DR_DIMENSION::k2D:
      dimUAV = D3D11_UAV_DIMENSION_TEXTURE2D;
      break;
    case DR_DIMENSION::k3D:
      dimUAV = D3D11_UAV_DIMENSION_TEXTURE3D;
      break;
    case DR_DIMENSION::kCUBE_MAP:
      dimUAV = D3D11_UAV_DIMENSION_UNKNOWN;
      break;
    default:
      break;
    }
    viewDescUAV.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
    viewDescUAV.Texture2D.MipSlice = 0;
    apiDevice->D3D11Device->CreateUnorderedAccessView(APITexture, &viewDescUAV, &m_APIUAV);
  }
  
  if (desc.CPUAccessFlags & DR_CPU_ACCESS_FLAG::drRead) {
    D3D11_TEXTURE2D_DESC apiDesc2 = apiDesc;
    apiDesc2.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_READ;
    apiDesc2.Usage = D3D11_USAGE_STAGING;;
    hr = apiDevice->
      D3D11Device->
      CreateTexture2D(&apiDesc2,
        0,
        &m_stagingTexture);
  }
}


void
D3DTexture::createEmpty(const Device& device, const DrTextureDesc& desc) {
  return createFromMemory(device, desc, 0);
}

void
D3DTexture::map(const DeviceContext& deviceContext, char** buffer) {
  D3D11_MAPPED_SUBRESOURCE mappedResource;
  HRESULT HR = reinterpret_cast<const D3DDeviceContext*>(&deviceContext)->
    D3D11DeviceContext->
      Map(APITexture,
          0,
          D3D11_MAP_WRITE_DISCARD,
          0,
          &mappedResource);

  *buffer = static_cast<char*>(mappedResource.pData);
}

void 
D3DTexture::getMemoryBuffer(const DeviceContext& deviceContext,
                            std::vector<byte>& buff) {
  if (!(m_descriptor.CPUAccessFlags & DR_CPU_ACCESS_FLAG::drRead)) {
    return;
  }

  ID3D11DeviceContext* dc = reinterpret_cast<const D3DDeviceContext*>(&deviceContext)->
    D3D11DeviceContext;
  dc->CopyResource(m_stagingTexture,APITexture);
  D3D11_MAPPED_SUBRESOURCE mappedResource;
  dc->Map(m_stagingTexture,
          0,
          D3D11_MAP_READ,
          0,
          &mappedResource);  
  buff.clear();
  buff.assign((byte*)mappedResource.pData, (byte*)mappedResource.pData + m_descriptor.pitch * m_descriptor.height);
  dc->Unmap(m_stagingTexture,0);
}

void
D3DTexture::set(const DeviceContext& deviceContext,
                UInt32 slot,
                DR_SHADER_TYPE_FLAG::E shaderType) const {
  if (DR_SHADER_TYPE_FLAG::kFragment == shaderType) {
    reinterpret_cast<const D3DDeviceContext*>(&deviceContext)->
      D3D11DeviceContext->
        PSSetShaderResources(slot, 1, &APIView);
  }
  else if (DR_SHADER_TYPE_FLAG::kCompute == shaderType) {
    if (m_descriptor.bindFlags & DR_BIND_FLAGS::UNORDERED_ACCESS) {
      reinterpret_cast<const D3DDeviceContext*>(&deviceContext)->
        D3D11DeviceContext->
          CSSetUnorderedAccessViews(slot, 1, &m_APIUAV, 0);
    }
    else {
      reinterpret_cast<const D3DDeviceContext*>(&deviceContext)->
        D3D11DeviceContext->
          CSSetShaderResources(slot, 1, &APIView);
    }
  }
}

void
D3DTexture::release() {
  APITexture->Release();
  if (m_descriptor.CPUAccessFlags & DR_CPU_ACCESS_FLAG::drRead)
    m_stagingTexture->Release();
  if (APIView)
    APIView->Release();
  if (m_descriptor.bindFlags & DR_BIND_FLAGS::UNORDERED_ACCESS) {
    m_APIUAV->Release();
  }
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
  Int32 buffSize = m_descriptor.pitch * m_descriptor.height; //This will crash on compressed textures, (h/4)
  char *pHead = const_cast<char*>(buffer);

  D3D11_TEXTURE2D_DESC pDesc;
  APITexture->GetDesc(&pDesc);

  for (UInt32 i = 0; i < m_arraySize; ++i) {
    reinterpret_cast<const D3DDeviceContext*>(&deviceContext)->
      D3D11DeviceContext->
      UpdateSubresource(APITexture, D3D11CalcSubresource(0, i, pDesc.MipLevels), 0, pHead, m_descriptor.pitch, 0);
      pHead += buffSize;
  }
  
}

void
D3DTexture::generateMipMaps(const DeviceContext& deviceContext) const {
  DR_ASSERT(m_descriptor.bindFlags != DR_BIND_FLAGS::DEPTH_STENCIL);
  reinterpret_cast<const D3DDeviceContext*>(&deviceContext)->
    D3D11DeviceContext->
    GenerateMips(APIView);
  
}

void 
D3DTexture::modifyTextureParams(const Device & device, const DrTextureDesc & desc) {
  APITexture->Release();
  if (APIView)
    APIView->Release();
  createFromMemory(device, desc, 0);
}
}