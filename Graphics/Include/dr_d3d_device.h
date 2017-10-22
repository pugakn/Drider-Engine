#pragma once
#include <dr_prerequisites.h>
#include "dr_device.h"
#include <wrl.h>
#include <wrl/client.h>
#include <D3Dcompiler.h>
#include <d3d11.h>
#include <dxgi.h>
namespace driderSDK {
class DR_API_EXPORT D3DDevice : public Device
{
public:
  void createVertexBuffer(const DrBufferDesc& desc, char* initialData, VertexBuffer& vertexBuffer) override;
  void createIndexBuffer(const DrBufferDesc& desc, char* initialData, IndexBuffer& indexBuffer) override;
  void createConstantBuffer(const DrBufferDesc& desc, char* initialData, ConstantBuffer& constantBuffer) override;
  void createShaderFromMemory(const char* shaderBuffer, Shader& shader) override;

  void createBlendState() override;
  void createSamplerState() override;
  void createRasteizerState() override;
  void createDepthStencilState() override;

  void createTexture1D(const char* buffer, Texture& texture) override;
  void createTexture2D(const char* buffer, Texture& texture) override;
  void createTexture3D(const char* buffer, Texture& texture) override;

  Microsoft::WRL::ComPtr<ID3D11Device> D3D11Device;
private:
};
}