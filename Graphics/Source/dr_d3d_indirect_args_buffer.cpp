#include "dr_d3d_indirect_args_buffer.h"
#include <d3d11.h>
#include <dxgi.h>
#include "dr_d3d_device.h"
#include "dr_d3d_device_context.h"
#include "dr_d3d_structure_buffer.h"
namespace driderSDK {

  void* D3DIndirectArgsBuffer::getAPIObject() const {
    return m_buffer;
  }

  void**
    D3DIndirectArgsBuffer::getAPIObjectReference()  {
    return reinterpret_cast<void**>(&m_buffer);
  }

  D3DIndirectArgsBuffer::D3DIndirectArgsBuffer() {
  }

  D3DIndirectArgsBuffer::~D3DIndirectArgsBuffer()
  {
  }

  void
    D3DIndirectArgsBuffer::create(const Device& device,
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

    if (m_descriptor.type == DR_BUFFER_TYPE::kINDIRECT_DRAW_INSTANCED_INDEXED) {
      bdesc.ByteWidth = 5 * sizeof(UINT);
    }
    bdesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS;
    bdesc.CPUAccessFlags = 0;
    bdesc.MiscFlags = D3D11_RESOURCE_MISC_DRAWINDIRECT_ARGS;

    D3D11_SUBRESOURCE_DATA subData = { &initialData[0], 0, 0 };

    if (initialData != nullptr) {
      apiDevice->D3D11Device->
        CreateBuffer(&bdesc, &subData, &m_buffer);
    }
    else {
      apiDevice->D3D11Device->
        CreateBuffer(&bdesc, nullptr, &m_buffer);
    }


    D3D11_UNORDERED_ACCESS_VIEW_DESC sbUAVDesc;
    ZeroMemory(&sbUAVDesc, sizeof(sbUAVDesc));
    sbUAVDesc.Buffer.FirstElement = 0;
    sbUAVDesc.Buffer.Flags = 0; 
    if (m_descriptor.type == DR_BUFFER_TYPE::kINDIRECT_DRAW_INSTANCED_INDEXED) {
      sbUAVDesc.Buffer.NumElements = 5;
    }
    sbUAVDesc.Format = DXGI_FORMAT_R32_UINT;
    sbUAVDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
    apiDevice->D3D11Device->CreateUnorderedAccessView(m_buffer,
      &sbUAVDesc, &m_pBufferUAV);
  }

  void 
  D3DIndirectArgsBuffer::set(const DeviceContext & deviceContext, Int32 typeFlag, Int32 startSlot) const
  {
    const D3DDeviceContext* context = reinterpret_cast<const D3DDeviceContext*>
      (&deviceContext);
    if (typeFlag == 0) {
      typeFlag |= DR_SHADER_TYPE_FLAG::kCompute;
    }
    if (typeFlag& DR_SHADER_TYPE_FLAG::kCompute) {
      context->D3D11DeviceContext->CSSetUnorderedAccessViews(startSlot, 1, &m_pBufferUAV,0);
    }
  }

 

  void
    D3DIndirectArgsBuffer::updateFromSysMemCpy(const DeviceContext& deviceContext) {
    reinterpret_cast<const D3DDeviceContext*>(&deviceContext)->
      D3D11DeviceContext->
      UpdateSubresource(m_buffer, 0, 0, &m_sysMemCpy[0], 0, 0);
  }

  void
    D3DIndirectArgsBuffer::updateFromBuffer(const DeviceContext& deviceContext,
      const byte* dataBuffer) {
    m_sysMemCpy.assign(dataBuffer, dataBuffer + m_descriptor.sizeInBytes);
    reinterpret_cast<const D3DDeviceContext*>(&deviceContext)->
      D3D11DeviceContext->
      UpdateSubresource(m_buffer, 0, 0, &dataBuffer[0], 0, 0);
  }
  void
    D3DIndirectArgsBuffer::release() {
    m_buffer->Release();
    m_pBufferUAV->Release();
    m_sysMemCpy.clear();
    delete this;
  }

}