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
  virtual void createVertexBuffer(const DrBufferDesc& desc,char* initialData,VertexBuffer& vertexBuffer) = 0;
  virtual void createIndexBuffer(const DrBufferDesc& desc, char* initialData, IndexBuffer& indexBuffer) = 0;
  virtual void createConstantBuffer(const DrBufferDesc& desc, char* initialData, ConstantBuffer& constantBuffer) = 0;

  virtual void createShaderFromMemory(const char* shaderBuffer, size_t bufferSize, Shader& shader) = 0;

  virtual void createBlendState() = 0;
  virtual void createSamplerState() = 0;
  virtual void createRasteizerState() = 0;

  virtual void createTexture1D(const char* buffer, Texture& texture) = 0;
  virtual void createTexture2D(const char* buffer, Texture& texture) = 0;
  virtual void createTexture3D(const char* buffer, Texture& texture) = 0;

  virtual void createDepthStencilState() = 0;

 private:

};
}