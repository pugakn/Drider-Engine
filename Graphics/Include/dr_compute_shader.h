#pragma once
#include "dr_shader.h"
namespace driderSDK {
/**
* Base class for compute shader
*
* Sample usage:
*	ComputeShader* = new D3DComputeShader;
*/
class DR_API_EXPORT ComputeShader : public Shader
{
public:
  virtual 
  ~ComputeShader() {}
};
}