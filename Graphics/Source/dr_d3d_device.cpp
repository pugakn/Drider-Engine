#include "dr_d3d_device.h"
#include "dr_d3d_vertex_buffer.h"
#include <iostream>
namespace driderSDK {
void D3DDevice::createVertexBuffer(const DrBufferDesc& desc, 
                                    char * initialData, 
                                    VertexBuffer& vertexBuffer) 
{
  D3D11_BUFFER_DESC bdesc = { 0 };
  bdesc.ByteWidth = desc.sizeInBytes;
  bdesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
  D3D11_SUBRESOURCE_DATA subData = { initialData, 0, 0 };

  if (D3D11Device->CreateBuffer(&bdesc, &subData, &(reinterpret_cast<D3DVertexBuffer*>(&vertexBuffer))->VB) != S_OK) {
    std::cout << "Error Creating VB" << std::endl;
    return;
  }
}
void D3DDevice::createIndexBuffer(const DrBufferDesc & desc, char * initialData, IndexBuffer & indexBuffer)
{
}
void D3DDevice::createConstantBuffer(const DrBufferDesc & desc, char * initialData, ConstantBuffer & constantBuffer)
{
}
void D3DDevice::createShaderFromMemory(const char * shaderBuffer, Shader & shader)
{
}
}



