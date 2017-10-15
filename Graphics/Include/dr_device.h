#pragma once
#include "dr_swap_chain.h"
#include <dr_prerequisites.h>
namespace driderSDK {
class Device
{
 public:
  virtual void init() = 0;
  virtual void destroy() = 0;

  virtual void createBuffer() = 0;

  virtual void createFragmentShader() = 0;
  virtual void crateVertexShader() = 0;
  virtual void createComputeShader() = 0;
  virtual void createGeometryShader() = 0;
  virtual void createHullShader() = 0;
  virtual void createTextureShader() = 0;
  virtual void createDomainShader() = 0;
  virtual void createTeselationShader() = 0;

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