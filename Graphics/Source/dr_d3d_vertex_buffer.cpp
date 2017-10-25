#include "dr_d3d_vertex_buffer.h"
#include <d3d11.h>
#include <dxgi.h>
#include "dr_d3d_device.h"
#include "dr_d3d_device_context.h"
namespace driderSDK {
D3DVertexBuffer::D3DVertexBuffer()
{
}

DR_GRAPHICS_ERROR::E D3DVertexBuffer::create(const Device& device, const DrBufferDesc & desc, char * initialData)
{
  D3D11_BUFFER_DESC bdesc = { 0 };
  switch (desc.usage)
  {
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
  D3D11_SUBRESOURCE_DATA subData = { initialData, 0, 0 };
  if (static_cast<const D3DDevice*>(&device)->D3D11Device->CreateBuffer(&bdesc, &subData, &VB) != S_OK) {
    return DR_GRAPHICS_ERROR::CREATE_BUFFER_ERROR;
  }
  return DR_GRAPHICS_ERROR::ERROR_NONE;
}

void D3DVertexBuffer::set(const DeviceContext& deviceContext, 
                          UInt32 stride, 
                          UInt32 offset) const {
  static_cast<const D3DDeviceContext*>(&deviceContext)->D3D11DeviceContext->IASetVertexBuffers(0, 
                                                                                               1, 
                                                                                               VB.GetAddressOf(), 
                                                                                               &stride, 
                                                                                               &offset);
}
void D3DVertexBuffer::release()
{
  VB.Reset();
}
}


