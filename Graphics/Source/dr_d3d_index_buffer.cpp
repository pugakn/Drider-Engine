#include "dr_d3d_index_buffer.h"
#include <d3d11.h>
#include <dxgi.h>
#include "dr_d3d_device.h"
#include "dr_d3d_device_context.h"

namespace driderSDK {

void*
D3DIndexBuffer::getAPIObject() {
  return IB;
}

void**
D3DIndexBuffer::getAPIObjectReference() {
  return reinterpret_cast<void**>(&IB);
}

D3DIndexBuffer::D3DIndexBuffer() {
}

void
D3DIndexBuffer::create(const Device& device,
                       const DrBufferDesc& desc,
                       const byte* initialData) {
  const D3DDevice* apiDevice = reinterpret_cast<const D3DDevice*>(&device);
  m_descriptor = desc;
  if (initialData != nullptr) {
    m_sysMemCpy.resize(desc.sizeInBytes);
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
  bdesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
  D3D11_SUBRESOURCE_DATA subData = { &initialData[0], 0, 0 };

  if (initialData != nullptr) {
    apiDevice->D3D11Device->
      CreateBuffer(&bdesc, &subData, &IB);
  }
  else {
    apiDevice->D3D11Device->
      CreateBuffer(&bdesc, nullptr, &IB);
  }
}

void
D3DIndexBuffer::set(const DeviceContext& deviceContext, UInt32 offset) const {
  reinterpret_cast<const D3DDeviceContext*>(&deviceContext)->
    D3D11DeviceContext->
      IASetIndexBuffer(IB, DXGI_FORMAT_R32_UINT, offset);
}

void
D3DIndexBuffer::updateFromSysMemCpy(const DeviceContext& deviceContext) {
  reinterpret_cast<const D3DDeviceContext*>(&deviceContext)->
    D3D11DeviceContext->
    UpdateSubresource(IB, 0, 0, &m_sysMemCpy[0], 0, 0);
}

void
D3DIndexBuffer::updateFromBuffer(const DeviceContext& deviceContext,
                                 const byte* dataBuffer) {
  m_sysMemCpy.assign(dataBuffer, dataBuffer + m_descriptor.sizeInBytes);
  reinterpret_cast<const D3DDeviceContext*>(&deviceContext)->
    D3D11DeviceContext->
      UpdateSubresource(IB, 0, 0, &dataBuffer[0], 0, 0);
}

void
D3DIndexBuffer::release() {
  IB->Release();
  m_sysMemCpy.clear();
  delete this;
}

}