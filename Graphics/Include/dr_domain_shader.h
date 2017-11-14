#pragma once
#include "dr_shader.h"

namespace driderSDK {

/**
* Base class for domain shader
*
* Sample usage:
*	DomainShader* = new D3DDomainShader;
*/
class DR_GRAPHICS_EXPORT DomainShader : public Shader
{
 public:
  /**
  * Virtual destructor.
  */
  virtual
  ~DomainShader() {}
};

}