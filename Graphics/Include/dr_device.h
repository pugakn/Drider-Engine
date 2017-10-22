#pragma once
#include <dr_prerequisites.h>
#include "dr_swap_chain.h"
#include "dr_vertex_shader.h"
#include "dr_fragment_shader.h"
#include "dr_compute_shader.h"
#include "dr_teselation_shader.h"
#include "dr_domain_shader.h"
#include "sr_texture_shader.h"
#include "dr_index_buffer.h"
#include "dr_vertex_buffer.h"
#include "dr_constant_buffer.h"
#include "dr_texture.h"
namespace driderSDK {
class DR_API_EXPORT Device
{
 public:
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