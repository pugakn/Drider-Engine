#include "dr_d3d_vertex_buffer.h"
#include "dr_d3d_device.h"
#include <iostream>

namespace driderSDK {
D3DVertexBuffer::D3DVertexBuffer()
{
}


D3DVertexBuffer::~D3DVertexBuffer()
{
}


void D3DVertexBuffer::create(const Device& device, const DrBufferDesc & desc, char * initialData)
{
  D3D11_BUFFER_DESC bdesc = { 0 };
  bdesc.ByteWidth = desc.sizeInBytes;
  bdesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
  D3D11_SUBRESOURCE_DATA subData = { initialData, 0, 0 };
  if (static_cast<const D3DDevice*>(&device)->D3D11Device->CreateBuffer(&bdesc, &subData, &VB) != S_OK) {
    std::cout << "Error Creating VB" << std::endl;
    return;
  }
}
}


