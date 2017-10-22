#include "dr_d3d_index_buffer.h"
#include <iostream>
#include "dr_d3d_device.h"
#include "dr_d3d_device_context.h"
namespace driderSDK {
D3DIndexBuffer::D3DIndexBuffer()
{
}


D3DIndexBuffer::~D3DIndexBuffer()
{
}

void D3DIndexBuffer::create(const Device& device, const DrBufferDesc & desc, char * initialData)
{
  D3D11_BUFFER_DESC bdesc = { 0 };
  bdesc.ByteWidth = desc.sizeInBytes;
  bdesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
  D3D11_SUBRESOURCE_DATA subData = { initialData, 0, 0 };
  if (static_cast<const D3DDevice*>(&device)->D3D11Device->CreateBuffer(&bdesc, &subData, &IB) != S_OK) {
    std::cout << "Error Creating Index Buffer" << std::endl;
  }
}

void D3DIndexBuffer::set(const DeviceContext& deviceContext, UInt32 offset)
{
  static_cast<const D3DDeviceContext*>(&deviceContext)->D3D11DeviceContext->IASetIndexBuffer(IB.Get(), DXGI_FORMAT_R16_UINT, offset);
}
}