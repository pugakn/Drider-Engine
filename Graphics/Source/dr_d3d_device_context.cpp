#include "dr_d3d_device_context.h"
#include <d3d11.h>
#include <dxgi.h>
#include "dr_shader.h"
#include "dr_d3d_vertex_buffer.h"
#include "dr_d3d_index_buffer.h"
namespace driderSDK {
void D3DDeviceContext::clearDepthStencilView(DepthStencil& depthstencil,
  DR_DEPTH_STENCIL_CLEAR_TYPE::E clearType,
  float depthValue,
  UInt32 stencilValue)
{
  UInt32 clearFlags = 0;
  switch (clearType)
  {
  case DR_DEPTH_STENCIL_CLEAR_TYPE::kClearDepth:
    clearFlags |= D3D11_CLEAR_DEPTH;
    break;
  case DR_DEPTH_STENCIL_CLEAR_TYPE::kClearStencil:
    clearFlags |= D3D11_CLEAR_STENCIL;
    break;
  case DR_DEPTH_STENCIL_CLEAR_TYPE::kClearDepthStencil:
    clearFlags |= D3D11_CLEAR_DEPTH;
    clearFlags |= D3D11_CLEAR_STENCIL;
    break;
  }
  D3D11DeviceContext->ClearDepthStencilView(depthstencil, clearFlags, depthValue, stencilValue);
}

void D3DDeviceContext::clearRenderTargetView(RenderTarget& renderTarget, const float colorRGBA[4])
{
  D3D11DeviceContext->ClearRenderTargetView(renderTarget, colorRGBA);
}

void D3DDeviceContext::setShader(const Shader& shader) const
{
  shader.set(*this);
}

void D3DDeviceContext::setIndexBuffer(const IndexBuffer& indexBuffer, UInt32 offset) const
{
  indexBuffer.set(*this,offset);
}
void D3DDeviceContext::setVertexBuffer(const VertexBuffer& buffer, UInt32 stride, UInt32 offset) const
{

  buffer.set(*this, stride, offset);
}

void D3DDeviceContext::setPrimitiveTopology(DR_PRIMITIVE_TOPOLOGY::E topology)
{

}
void D3DDeviceContext::draw()
{

}

}