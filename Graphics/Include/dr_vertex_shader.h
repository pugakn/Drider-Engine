#pragma once
#include "dr_shader.h"
namespace driderSDK {
/**
* Base class for vertex shader
*
* Sample usage:
*	VertexShader* = new D3DVertexShader;
*/
class DR_API_EXPORT VertexShader : public Shader
{
public:
  virtual 
  ~VertexShader() {}
};
}