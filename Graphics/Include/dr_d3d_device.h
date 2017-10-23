#pragma once
#include <dr_prerequisites.h>
#include "dr_device.h"
#include <wrl.h>
#include <wrl/client.h>
namespace driderSDK {
class DR_API_EXPORT DR_API_EXPORT D3DDevice : public Device
{
public:
  DR_GRAPHICS_ERROR::E createVertexBuffer(const DrBufferDesc& desc, char* initialData, VertexBuffer& vertexBuffer) override;
  DR_GRAPHICS_ERROR::E createIndexBuffer(const DrBufferDesc& desc, char* initialData, IndexBuffer& indexBuffer) override;
  DR_GRAPHICS_ERROR::E createConstantBuffer(const DrBufferDesc& desc, char* initialData, ConstantBuffer& constantBuffer) override;
  DR_GRAPHICS_ERROR::E createShaderFromMemory(const char* shaderBuffer, size_t bufferSize, Shader& shader) override;

  DR_GRAPHICS_ERROR::E createBlendState() override;
  DR_GRAPHICS_ERROR::E createSamplerState() override;
  DR_GRAPHICS_ERROR::E createRasteizerState() override;
  DR_GRAPHICS_ERROR::E createDepthStencilState() override;

  DR_GRAPHICS_ERROR::E createTexture(const char* buffer, const DrTextureDesc& desc,Texture& texture) override;

  Microsoft::WRL::ComPtr<ID3D11Device> D3D11Device;
private:
};
}