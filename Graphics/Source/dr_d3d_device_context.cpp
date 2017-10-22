#include "dr_d3d_device_context.h"
#include "dr_shader.h"
#include "dr_d3d_vertex_buffer.h"
#include "dr_d3d_index_buffer.h"
namespace driderSDK {
void D3DDeviceContext::clearDepthStencilView(DepthStencil& depthstencil,
  DR_DEPTH_STENCIL_CLEAR_TYPE::E clearType,
  float depthValue,
  UInt32 stencilValue)
{
  D3D11DeviceContext->ClearDepthStencilView(depthstencil, , depthValue, stencilValue);
}

void D3DDeviceContext::clearRenderTargetView(RenderTarget& renderTarget, const float colorRGBA[4])
{
  D3D11DeviceContext->ClearRenderTargetView(renderTarget, colorRGBA);
}

void D3DDeviceContext::setShader(const Shader& shader) 
{
  shader.set(*this);
}

void D3DDeviceContext::setIndexBuffer(const IndexBuffer& indexBuffer, UInt32 offset)
{
  indexBuffer.set(*this,offset);
}
void D3DDeviceContext::setVertexBuffers(UInt32 slot, UInt32 numBuffers, const VertexBuffer& buffer, UInt32 stride, UInt32 offset)
{
  buffer.set(*this,slot, numBuffers, stride, offset);
}

void D3DDeviceContext::setPrimitiveTopology(DR_PRIMITIVE_TOPOLOGY::E topology)
{

}
void D3DDeviceContext::draw()
{

}

}