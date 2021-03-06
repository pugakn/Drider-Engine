#include "dr_d3d_device_context.h"
#include <d3d11.h>
#include <dxgi.h>
#include "dr_shader.h"
#include "dr_d3d_vertex_buffer.h"
#include "dr_d3d_index_buffer.h"
#include "dr_d3d_constant_buffer.h"
#include "dr_shader.h"
#include "dr_d3d_device_context.h"
#include "dr_d3d_texture.h"
#include "dr_d3d_render_target.h"
#include "dr_d3d_depth_stencil.h"
#include "dr_d3d_sample_state.h"
#include "dr_d3d_depth_stencil_state.h"
#include "dr_d3d_rasterizer_state.h"
#include "dr_d3d_input_layout.h"
#include "dr_d3d_swap_chain.h"

namespace driderSDK {

struct DeviceContextData {
  ID3D11DeviceContext* pDeviceContext;
  void*  getObject() {};
  void** getObjectReference() {};
};

void
D3DDeviceContext::clearDepthStencilView(DepthStencil& depthstencil,
                                        DR_DEPTH_STENCIL_CLEAR_TYPE::E clearType,
                                        float depthValue,
                                        UInt32 stencilValue) const {
  UInt32 clearFlags = 0;

  switch (clearType) {
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
  default:
    break;
  }

  D3D11DeviceContext->
    ClearDepthStencilView(reinterpret_cast<D3DDepthStencil*>(&depthstencil)->APIDepthView,
                          clearFlags,
                          depthValue,
                          static_cast<driderSDK::UInt8>(stencilValue));
}

void
D3DDeviceContext::clearRenderTargetView(RenderTarget& renderTarget,
                                        const float colorRGBA[4]) const {
  for (auto&it : static_cast<D3DRenderTarget*>(&renderTarget)->RTVs) {
    D3D11DeviceContext->
      ClearRenderTargetView(it, colorRGBA);
  }
}

void* D3DDeviceContext::getAPIObject() {
  return D3D11DeviceContext;
}

void**
D3DDeviceContext::getAPIObjectReference() {
  return reinterpret_cast<void**>(&D3D11DeviceContext);
}

void
D3DDeviceContext::release() {
  D3D11DeviceContext->Release();
  delete this;
}

void
D3DDeviceContext::generateMipMaps(const Texture& texture) const {
  texture.generateMipMaps(*this);
}

void
D3DDeviceContext::updateTextureFromMemory(Texture& texture,
                                          const char* databuffer,
                                          size_t bufferSize) const {
  texture.udpateFromMemory(*this, databuffer, bufferSize);
}

void
D3DDeviceContext::updateBufferFromMemory(Buffer& buffer,
                                         const byte* dataBuffer) const {
  buffer.updateFromBuffer(*this, dataBuffer);
}

void
D3DDeviceContext::setRenderTarget(const RenderTarget& renderTarget,
                                  const DepthStencil& depthStencil) const {
  renderTarget.set(*this, depthStencil);
}

void
D3DDeviceContext::setTexture(const Texture& texture, UInt32 slot) const {
  texture.set(*this,slot);
}

void
D3DDeviceContext::setShader(const Shader& shader) const {
  shader.set(*this);
}

void
D3DDeviceContext::setIndexBuffer(const IndexBuffer& indexBuffer,
                                 UInt32 offset) const {
  indexBuffer.set(*this,offset);
}

void
D3DDeviceContext::setVertexBuffer(const VertexBuffer& buffer,
                                  UInt32 stride,
                                  UInt32 offset) const {
  buffer.set(*this, offset);
}

void
D3DDeviceContext::setConstantBuffer(const ConstantBuffer& buffer,
                                    DR_SHADER_TYPE_FLAG::E typeFlag) const {
  buffer.set(*this, typeFlag);
}

void
D3DDeviceContext::setPrimitiveTopology(DR_PRIMITIVE_TOPOLOGY::E topology) const {
  D3D11_PRIMITIVE_TOPOLOGY topo = D3D11_PRIMITIVE_TOPOLOGY_UNDEFINED;

  switch (topology)
  {
  case driderSDK::DR_PRIMITIVE_TOPOLOGY::kLineList:
    topo = D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
    break;
  case driderSDK::DR_PRIMITIVE_TOPOLOGY::kTriangleList:
    topo = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
    break;
  case driderSDK::DR_PRIMITIVE_TOPOLOGY::kLineStrip:
    topo = D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP;
    break;
  case driderSDK::DR_PRIMITIVE_TOPOLOGY::kTriangleStrip:
    topo = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
    break;
  default:
    break;
  }

  D3D11DeviceContext->IASetPrimitiveTopology(topo);
}

void
D3DDeviceContext::draw(UInt32 indexCount,
                       UInt32 startIndexLocation,
                       UInt32 startVertexLocation) const {
  D3D11DeviceContext->DrawIndexed(indexCount, startIndexLocation, startVertexLocation);
}

}