#include "dr_d3d_structure_buffer.h"
#include <d3d11.h>
#include <dxgi.h>
#include "dr_d3d_device.h"
#include "dr_d3d_device_context.h"

namespace driderSDK {

  void* D3DStructureBuffer::getAPIObject() const {
    return CB;
  }

  void**
    D3DStructureBuffer::getAPIObjectReference()  {
    return reinterpret_cast<void**>(&CB);
  }

  D3DStructureBuffer::D3DStructureBuffer() {
  }

  D3DStructureBuffer::~D3DStructureBuffer()
  {
  }

  void
    D3DStructureBuffer::create(const Device& device,
      const DrBufferDesc& desc,
      const byte* initialData) {
    const D3DDevice* apiDevice = reinterpret_cast<const D3DDevice*>(&device);
    m_descriptor = desc;
    m_sysMemCpy.resize(desc.sizeInBytes);
    if (initialData != nullptr) {
      m_sysMemCpy.assign(initialData, initialData + desc.sizeInBytes);
    }
    D3D11_BUFFER_DESC bdesc = { 0 };

    switch (desc.usage) {
    case DR_BUFFER_USAGE::kDefault:
      bdesc.Usage = D3D11_USAGE_DEFAULT;
      break;
    case DR_BUFFER_USAGE::kDynamic:
      bdesc.Usage = D3D11_USAGE_DYNAMIC;
      break;
    case DR_BUFFER_USAGE::kImmutable:
      bdesc.Usage = D3D11_USAGE_IMMUTABLE;
      break;
    default:
      break;
    }

    bdesc.ByteWidth = desc.sizeInBytes;
    if (m_descriptor.type == DR_BUFFER_TYPE::kRWSTRUCTURE)
      bdesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
    else 
      bdesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    bdesc.CPUAccessFlags = 0;
    bdesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
    bdesc.StructureByteStride = desc.stride;

    D3D11_SUBRESOURCE_DATA subData = { &initialData[0], 0, 0 };

    if (initialData != nullptr) {
      apiDevice->D3D11Device->
        CreateBuffer(&bdesc, &subData, &CB);
    }
    else {
      apiDevice->D3D11Device->
        CreateBuffer(&bdesc, nullptr, &CB);
    }

    //Read 
    D3D11_SHADER_RESOURCE_VIEW_DESC descView;
    ZeroMemory(&descView, sizeof(descView));
    descView.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
    descView.BufferEx.FirstElement = 0;
    descView.Format = DXGI_FORMAT_UNKNOWN;
    descView.BufferEx.NumElements = desc.sizeInBytes / desc.stride;

    apiDevice->D3D11Device->CreateShaderResourceView(CB,
      &descView, &m_pBufferView);

    if (desc.type == DR_BUFFER_TYPE::kRWSTRUCTURE) {
      //Write
      D3D11_UNORDERED_ACCESS_VIEW_DESC sbUAVDesc;
      ZeroMemory(&sbUAVDesc, sizeof(sbUAVDesc));
      sbUAVDesc.Buffer.FirstElement = 0;
      sbUAVDesc.Buffer.Flags = D3D11_BUFFER_UAV_FLAG_APPEND; //TODO: config
      sbUAVDesc.Buffer.NumElements = desc.sizeInBytes/desc.stride;
      sbUAVDesc.Format = DXGI_FORMAT_UNKNOWN;
      sbUAVDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
      apiDevice->D3D11Device->CreateUnorderedAccessView(CB,
        &sbUAVDesc, &m_pBufferUAV);
    }
  }

  void
    D3DStructureBuffer::set(const DeviceContext& deviceContext,
      Int32 typeFlag, Int32 startSlot , UInt32 _resetCounter) const {
    const D3DDeviceContext* context = reinterpret_cast<const D3DDeviceContext*>
      (&deviceContext);
    if (typeFlag == 0) {
      typeFlag |= DR_SHADER_TYPE_FLAG::kVertex;
      typeFlag |= DR_SHADER_TYPE_FLAG::kFragment;
    }
    if (typeFlag& DR_SHADER_TYPE_FLAG::kVertex) {
      context->D3D11DeviceContext->VSSetShaderResources(startSlot, 1, &m_pBufferView);
    }
    if (typeFlag& DR_SHADER_TYPE_FLAG::kFragment) {
      context->D3D11DeviceContext->PSSetShaderResources(startSlot, 1, &m_pBufferView);
    }
    if (typeFlag& DR_SHADER_TYPE_FLAG::kCompute) {
      if (m_descriptor.type == DR_BUFFER_TYPE::kRWSTRUCTURE) {
        context->D3D11DeviceContext->CSSetUnorderedAccessViews(startSlot, 1, &m_pBufferUAV, &_resetCounter);
      }
      else 
        context->D3D11DeviceContext->CSSetShaderResources(startSlot, 1, &m_pBufferView);
    }
    if (typeFlag& DR_SHADER_TYPE_FLAG::kDomain) {
      context->D3D11DeviceContext->DSSetShaderResources(startSlot, 1, &m_pBufferView);
    }
    if (typeFlag& DR_SHADER_TYPE_FLAG::kHull) {
      context->D3D11DeviceContext->HSSetShaderResources(startSlot, 1, &m_pBufferView);
    }
    if (typeFlag& DR_SHADER_TYPE_FLAG::kGeometry) {
      context->D3D11DeviceContext->GSSetShaderResources(startSlot, 1, &m_pBufferView);
    }
    if (typeFlag & DR_SHADER_TYPE_FLAG::kTeselation) {
      //Not implemented
    }
    if (typeFlag & DR_SHADER_TYPE_FLAG::kTexture) {
      //Not implemented
    }
  }

  void
    D3DStructureBuffer::updateFromSysMemCpy(const DeviceContext& deviceContext) {
    reinterpret_cast<const D3DDeviceContext*>(&deviceContext)->
      D3D11DeviceContext->
      UpdateSubresource(CB, 0, 0, &m_sysMemCpy[0], 0, 0);
  }

  void
    D3DStructureBuffer::updateFromBuffer(const DeviceContext& deviceContext,
      const byte* dataBuffer) {
    m_sysMemCpy.assign(dataBuffer, dataBuffer + m_descriptor.sizeInBytes);
    reinterpret_cast<const D3DDeviceContext*>(&deviceContext)->
      D3D11DeviceContext->
      UpdateSubresource(CB, 0, 0, &dataBuffer[0], 0, 0);
  }
  void
    D3DStructureBuffer::release() {
    CB->Release();
    m_pBufferView->Release();
    if (m_descriptor.type == DR_BUFFER_TYPE::kRWSTRUCTURE) {
      m_pBufferUAV->Release();
    }
    m_sysMemCpy.clear();
    delete this;
  }

}