#pragma once
#include <dr_prerequisites.h>
#include "dr_graphics_defines.h"
namespace driderSDK {
class VertexBuffer;
class IndexBuffer;
class ConstantBuffer;
class Shader;
class Texture;
class SamplerState;
class RasterizerState;
class DepthStencilState;
class InputLayout;
class SwapChain;
class RenderTarget;
class DepthStencil;
class DeviceContext;
class DR_API_EXPORT Device
{
 public:
  virtual 
  ~Device() {}

  virtual DR_GRAPHICS_ERROR::E 
  createDeviceAndDeviceContext(DeviceContext& deviceContext) = 0;

  /**
  * Release the allocated memory
  */
  virtual void 
  release() = 0;


  virtual DR_GRAPHICS_ERROR::E 
  createVertexBuffer(const DrBufferDesc& desc,
                     char* initialData,
                     VertexBuffer& vertexBuffer) = 0;

  virtual DR_GRAPHICS_ERROR::E 
  createIndexBuffer(const DrBufferDesc& desc, 
                    char* initialData, 
                    IndexBuffer& indexBuffer) = 0;

  virtual DR_GRAPHICS_ERROR::E 
  createConstantBuffer(const DrBufferDesc& desc, 
                       char* initialData, 
                       ConstantBuffer& constantBuffer) = 0;

  virtual DR_GRAPHICS_ERROR::E 
  createShaderFromMemory(const char* shaderBuffer, 
                         size_t bufferSize, 
                         Shader& shader) = 0;

  virtual DR_GRAPHICS_ERROR::E 
  createTextureFromMemory(const char* buffer, 
                          const DrTextureDesc& desc, 
                          Texture& texture) = 0;

  virtual DR_GRAPHICS_ERROR::E 
  createEmptyTexture(const DrTextureDesc& desc, 
                     Texture& texture) = 0;

  virtual DR_GRAPHICS_ERROR::E 
  createRenderTarget(const DrTextureDesc& desc, 
                     RenderTarget& renderTarget, 
                     UInt32 _numColorTextures) = 0;

  virtual DR_GRAPHICS_ERROR::E 
  createDepthStencil(const DrTextureDesc& desc, 
                     DepthStencil& depthStencil) = 0;

  virtual DR_GRAPHICS_ERROR::E 
  createSamplerState(const DrSampleDesc& desc, 
                     SamplerState& state) = 0;

  virtual DR_GRAPHICS_ERROR::E 
  createRasteizerState(const DrRasterizerDesc& desc, 
                       RasterizerState& state) = 0;

  virtual DR_GRAPHICS_ERROR::E 
  createDepthStencilState(const DrDepthStencilDesc& desc, 
                          DepthStencilState& state) = 0;

  virtual DR_GRAPHICS_ERROR::E 
  createInputLayout(const DrInputElementDesc* inputDescArray, 
                    UInt32 arraySize, 
                    InputLayout& layout) = 0;

  virtual DR_GRAPHICS_ERROR::E 
  createSwapChain(const DrSwapChainDesc& desc, 
                  SwapChain& swapChain) = 0;
};
}