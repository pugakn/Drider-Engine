#pragma once
#include <dr_prerequisites.h>
#include "dr_swap_chain.h"
#include "dr_vertex_shader.h"
#include "dr_fragment_shader.h"
#include "dr_compute_shader.h"
#include "dr_teselation_shader.h"
#include "dr_domain_shader.h"
#include "sr_texture_shader.h"

namespace driderSDK {
class Device
{
 public:
  virtual void init() = 0;
  virtual void destroy() = 0;

  virtual void createBuffer() = 0;

  virtual void createShader(char* shaderBuffer, VertexShader* shader) = 0;
  virtual void createShader(char* shaderBuffer, FragmentShader* shader) = 0;
  virtual void createShader(char* shaderBuffer, ComputeShader* shader) = 0;
  virtual void createShader(char* shaderBuffer, TeselationShader* shader) = 0;
  virtual void createShader(char* shaderBuffer, DomainShader* shader) = 0;
  virtual void createShader(char* shaderBuffer, TextureShader* shader) = 0;

  virtual void createBlendState() = 0;
  virtual void createSamplerState() = 0;
  virtual void createRasteizerState() = 0;

  virtual void createTexture1D() = 0;
  virtual void createTexture2D() = 0;
  virtual void createTexture3D() = 0;

  virtual void createDepthStencilView() = 0;
  virtual void createDepthStencilState() = 0;

 private:

};
}