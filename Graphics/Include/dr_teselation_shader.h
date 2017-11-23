#pragma once
#include "dr_shader.h"

namespace driderSDK {

/**
* Base class for teselation shader
*
* Sample usage:
*	TeselationShader* = new D3DTeselationShader;
*/
class DR_GRAPHICS_EXPORT TeselationShader : public Shader
{
 public:
  /**
  * Virtual destructor.
  */
  virtual
  ~TeselationShader() {}
};

}