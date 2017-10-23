#pragma once
#include <dr_prerequisites.h>
#include "dr_graphics_defines.h"
namespace driderSDK {
class VertexBuffer;
class IndexBuffer;
class ConstantBuffer;
class Shader;
class Texture;
class DR_API_EXPORT Device
{
 public:
  virtual ~Device() {}
  virtual DR_GRAPHICS_ERROR::E create() = 0;
  virtual void release() = 0;

  virtual DR_GRAPHICS_ERROR::E createVertexBuffer(const DrBufferDesc& desc,char* initialData,VertexBuffer& vertexBuffer) = 0;
  virtual DR_GRAPHICS_ERROR::E createIndexBuffer(const DrBufferDesc& desc, char* initialData, IndexBuffer& indexBuffer) = 0;
  virtual DR_GRAPHICS_ERROR::E createConstantBuffer(const DrBufferDesc& desc, char* initialData, ConstantBuffer& constantBuffer) = 0;

  virtual DR_GRAPHICS_ERROR::E createShaderFromMemory(const char* shaderBuffer, size_t bufferSize, Shader& shader) = 0;

  virtual DR_GRAPHICS_ERROR::E createBlendState() = 0;
  virtual DR_GRAPHICS_ERROR::E createSamplerState() = 0;
  virtual DR_GRAPHICS_ERROR::E createRasteizerState() = 0;

  virtual DR_GRAPHICS_ERROR::E createTexture(const char* buffer, const DrTextureDesc& desc, Texture& texture) = 0;

  virtual DR_GRAPHICS_ERROR::E createDepthStencilState() = 0;

 private:

};
}