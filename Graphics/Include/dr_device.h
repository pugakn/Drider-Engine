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

namespace driderSDK {
class Device
{
 public:
  virtual void createVertexBuffer(const DrBufferDesc& desc,char* initialData,VertexBuffer* vertexBuffer) = 0;
  virtual void createIndexBuffer(const DrBufferDesc& desc, char* initialData, IndexBuffer* indexBuffer) = 0;
  virtual void createConstantBuffer(const DrBufferDesc& desc, char* initialData, ConstantBuffer* constantBuffer) = 0;

  virtual void createShader(const char* shaderBuffer, VertexShader* shader) = 0;
  virtual void createShader(const char* shaderBuffer, FragmentShader* shader) = 0;
  virtual void createShader(const char* shaderBuffer, ComputeShader* shader) = 0;
  virtual void createShader(const char* shaderBuffer, TeselationShader* shader) = 0;
  virtual void createShader(const char* shaderBuffer, DomainShader* shader) = 0;
  virtual void createShader(const char* shaderBuffer, TextureShader* shader) = 0;

  virtual void createBlendState() = 0;
  virtual void createSamplerState() = 0;
  virtual void createRasteizerState() = 0;

  virtual void createTexture1D() = 0;
  virtual void createTexture2D() = 0;
  virtual void createTexture3D() = 0;

  virtual void createDepthStencilState() = 0;

 private:

};
}