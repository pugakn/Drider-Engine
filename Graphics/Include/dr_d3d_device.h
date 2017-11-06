#pragma once
#include <dr_prerequisites.h>
#include "dr_device.h"

class ID3D11Device;

namespace driderSDK {

class DR_API_EXPORT D3DDevice : public Device
{
public:
  DR_GRAPHICS_ERROR::E
  createDeviceAndDeviceContext(DeviceContext& deviceContext) override;

  void
  release() override;

  DR_GRAPHICS_ERROR::E
  createVertexBuffer(const DrBufferDesc& desc,
                     char* initialData,
                     VertexBuffer& vertexBuffer) override;

  DR_GRAPHICS_ERROR::E
  createIndexBuffer(const DrBufferDesc& desc,
                    char* initialData,
                    IndexBuffer& indexBuffer) override;

  DR_GRAPHICS_ERROR::E
  createConstantBuffer(const DrBufferDesc& desc,
                       char* initialData,
                       ConstantBuffer& constantBuffer) override;

  DR_GRAPHICS_ERROR::E
  createShaderFromMemory(const char* shaderBuffer,
                         size_t bufferSize,
                         Shader& shader) override;

  DR_GRAPHICS_ERROR::E
  createTextureFromMemory(const char* buffer,
                          const DrTextureDesc& desc,
                          Texture& texture) override;

  DR_GRAPHICS_ERROR::E
  createEmptyTexture(const DrTextureDesc& desc,
                     Texture& texture) override;

  DR_GRAPHICS_ERROR::E
  createRenderTarget(const DrTextureDesc& desc,
                     RenderTarget& renderTarget,
                     UInt32 _numColorTextures) override;

  DR_GRAPHICS_ERROR::E
  createDepthStencil(const DrTextureDesc& desc,
                     DepthStencil& depthStencil) override;

  DR_GRAPHICS_ERROR::E
  createSamplerState(const DrSampleDesc& desc,
                     SamplerState& state) override;

  DR_GRAPHICS_ERROR::E
  createRasteizerState(const DrRasterizerDesc& desc,
                       RasterizerState& state) override;

  DR_GRAPHICS_ERROR::E
  createDepthStencilState(const DrDepthStencilDesc& desc,
                          DepthStencilState& state) override;

  DR_GRAPHICS_ERROR::E
  createInputLayout(const DrInputElementDesc* inputDescArray,
                    UInt32 arraySize,
                    InputLayout& layout) override;

  DR_GRAPHICS_ERROR::E
  createSwapChain(const DrSwapChainDesc& desc,
                  SwapChain& swapChain) override;

  ID3D11Device* D3D11Device;
};

}