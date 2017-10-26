#pragma once
#include <wrl.h>
#include <wrl/client.h>
#include "dr_device_context.h"
class ID3D11DeviceContext;
namespace driderSDK {
class DR_API_EXPORT D3DDeviceContext : public DeviceContext {
public:
  void release() override;

  void generateMipMaps(const Texture& texture) const override;
  void updateTextureFromMemory(Texture& texture, const char* databuffer, size_t bufferSize) const override;
  void updateBufferFromMemory(Buffer& buffer, const char* dataBuffer, size_t bufferSize) const override;

  void setRenderTarget(const RenderTarget& renderTarget, const DepthStencil& depthStencil) const override;
  void setTexture(const Texture& texture, UInt32 slot) const override;
  void setShader(const Shader& shader) const override;
  void setIndexBuffer(const IndexBuffer& indexBuffer, UInt32 offset) const override;
  void setVertexBuffer(const VertexBuffer& buffer, UInt32 stride, UInt32 offset) const override;
  void setConstantBuffer(const ConstantBuffer& buffer, DR_SHADER_TYPE_FLAG::E typeFlag) const override;

  void clearDepthStencilView(DepthStencil& depthstencil, DR_DEPTH_STENCIL_CLEAR_TYPE::E clearType, float depthValue, UInt32 stencilValue) const override;
  void clearRenderTargetView(RenderTarget& renderTarget, const float colorRGBA[4]) const override;
  void setPrimitiveTopology(DR_PRIMITIVE_TOPOLOGY::E topology) const override;
  void draw(UInt32 indexCount, UInt32 startIndexLocation, UInt32 startVertexLocation) const override;

  Microsoft::WRL::ComPtr<ID3D11DeviceContext> D3D11DeviceContext;
};
}