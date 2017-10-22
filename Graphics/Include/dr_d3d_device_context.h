#pragma once
#include <wrl.h>
#include <wrl/client.h>
#include <d3d11.h>
#include <dxgi.h>
#include "dr_device_context.h"
namespace driderSDK {
class D3DDeviceContext : public DeviceContext {
public:

  void clearDepthStencilView(DepthStencil& depthstencil, 
                             DR_DEPTH_STENCIL_CLEAR_TYPE::E clearType, 
                             float depthValue, 
                             UInt32 stencilValue) override;

  void clearRenderTargetView(RenderTarget& renderTarget, const float colorRGBA[4]) override;

  void generateMipMaps(const Texture& texture) override;
  void updateTextureFromMemory(const Texture& texture, const char* databuffer, size_t bufferSize) override;

  void setShader(const Shader& shader) override;

  void updateBufferFromMemory(const Buffer& buffer, const char* dataBuffer, size_t bufferSize) override;

  void setIndexBuffer(const IndexBuffer& indexBuffer, UInt32 offset) override;
  void setVertexBuffers(UInt32 slot, UInt32 numBuffers, const VertexBuffer& buffer, UInt32 stride, UInt32 offset) override;

  void setPrimitiveTopology(DR_PRIMITIVE_TOPOLOGY::E topology) override;
  void draw() override;


  Microsoft::WRL::ComPtr<ID3D11DeviceContext> D3D11DeviceContext;
};
}