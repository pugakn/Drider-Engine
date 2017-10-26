#pragma once
#include "dr_shader.h"
namespace driderSDK {
/**
* Base class for teselation shader
*
* Sample usage:
*	TeselationShader* = new D3DTeselationShader;
*/
class DR_API_EXPORT TeselationShader : public Shader
{
public:
  virtual 
  ~TeselationShader() {}
};
}