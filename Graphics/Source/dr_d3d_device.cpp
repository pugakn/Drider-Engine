#include "dr_d3d_device.h"
#include <iostream>
#include <D3Dcompiler.h>
#include <d3d11.h>
#include <dxgi.h>
#include "dr_d3d_vertex_buffer.h"
#include "dr_d3d_index_buffer.h"
#include "dr_d3d_constant_buffer.h"
#include "dr_shader.h"
namespace driderSDK {
DR_GRAPHICS_ERROR::E D3DDevice::createVertexBuffer(const DrBufferDesc& desc, 
                                    char * initialData, 
                                    VertexBuffer& vertexBuffer) 
{
  vertexBuffer.create(*this,desc,initialData);
}
DR_GRAPHICS_ERROR::E D3DDevice::createIndexBuffer(const DrBufferDesc & desc, char * initialData, IndexBuffer & indexBuffer)
{
  indexBuffer.create(*this, desc, initialData);
}
DR_GRAPHICS_ERROR::E D3DDevice::createConstantBuffer(const DrBufferDesc & desc, char * initialData, ConstantBuffer & constantBuffer)
{
  constantBuffer.create(*this, desc, initialData);
}
DR_GRAPHICS_ERROR::E D3DDevice::createShaderFromMemory(const char * shaderBuffer,size_t bufferSize, Shader & shader)
{
  shader.createFromMemory(*this,shaderBuffer,bufferSize);
}

}



