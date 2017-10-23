#pragma once
#include <wrl.h>
#include <wrl/client.h>
#include <d3d11.h>
#include <dxgi.h>
#include "dr_device_context.h"
#include "dr_shader.h"
#include "dr_buffer.h"
namespace driderSDK {
class DR_API_EXPORT D3DDeviceContext : public DeviceContext {
public:

  void create() override;
  void release() override;

  void generateMipMaps(const Texture& texture) override;
  void updateTextureFromMemory(const Texture& texture, const char* databuffer, size_t bufferSize) override;
  void updateBufferFromMemory(const Buffer& buffer, const char* dataBuffer, size_t bufferSize) override;

  void setRenderTargets() const override;
  void setTexture(Texture* texture) const override;
  void setShader(const Shader& shader) const override;
  void setIndexBuffer(const IndexBuffer& indexBuffer, UInt32 offset) const override;
  void setVertexBuffer(const VertexBuffer& buffer, UInt32 stride, UInt32 offset) const override;
  void setConstantBuffer(DR_SHADER_TYPE_FLAG::E typeFlag) const override;

  void clearDepthStencilView(DepthStencil& depthstencil, DR_DEPTH_STENCIL_CLEAR_TYPE::E clearType, float depthValue, UInt32 stencilValue) override;
  void clearRenderTargetView(RenderTarget& renderTarget, const float colorRGBA[4]) override;
  void setPrimitiveTopology(DR_PRIMITIVE_TOPOLOGY::E topology) override;
  void draw() override;


  Microsoft::WRL::ComPtr<ID3D11DeviceContext> D3D11DeviceContext;
};
}