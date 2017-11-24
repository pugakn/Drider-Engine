#include "dr_d3d_vertex_buffer.h"
#include <d3d11.h>
#include <dxgi.h>
#include "dr_d3d_device.h"
#include "dr_d3d_device_context.h"

namespace driderSDK {

D3DVertexBuffer::D3DVertexBuffer() {
}

DR_GRAPHICS_ERROR::E
D3DVertexBuffer::create(const Device& device,
                        const DrBufferDesc& desc,
                        const byte* initialData) {
  descriptor = desc;
  if (initialData != nullptr) {
    sysMemCpy.resize(desc.sizeInBytes);
    sysMemCpy.assign(initialData, initialData + desc.sizeInBytes);
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
  }

  bdesc.ByteWidth = desc.sizeInBytes;
  bdesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

  if (initialData != nullptr) {
    D3D11_SUBRESOURCE_DATA subData = { &initialData[0], 0, 0 };
    reinterpret_cast<const D3DDevice*>(&device)->D3D11Device->
      CreateBuffer(&bdesc, &subData, &VB);
  }
  else
  {
    reinterpret_cast<const D3DDevice*>(&device)->D3D11Device->
      CreateBuffer(&bdesc, nullptr, &VB);
  }
  
  return DR_GRAPHICS_ERROR::ERROR_NONE;
}

void
D3DVertexBuffer::set(const DeviceContext& deviceContext, 
                     UInt32 stride, 
                     UInt32 offset) const {
  reinterpret_cast<const D3DDeviceContext*>(&deviceContext)->
    D3D11DeviceContext->
      IASetVertexBuffers(0, 
                         1, 
                         &VB, 
                         &stride, 
                         &offset);
}

void
D3DVertexBuffer::updateFromSysMemCpy(const DeviceContext& deviceContext) {
  reinterpret_cast<const D3DDeviceContext*>(&deviceContext)->
    D3D11DeviceContext->
    UpdateSubresource(VB, 0, 0, &sysMemCpy[0], 0, 0);
}

void
D3DVertexBuffer::updateFromBuffer(const DeviceContext& deviceContext,
                                  const byte* dataBuffer) {
  sysMemCpy.assign(dataBuffer, dataBuffer + descriptor.sizeInBytes);
  reinterpret_cast<const D3DDeviceContext*>(&deviceContext)->
    D3D11DeviceContext->
      UpdateSubresource(VB, 0, 0, &dataBuffer[0], 0, 0);
}

void
D3DVertexBuffer::release() {
  VB->Release();
  sysMemCpy.clear();
  delete this;
}

}