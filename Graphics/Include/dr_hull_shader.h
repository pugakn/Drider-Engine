#pragma once
#include "dr_shader.h"

namespace driderSDK {

/**
* Base class for hull shader
*
* Sample usage:
*	HullShader* = new D3DhullShader;
*/
class DR_GRAPHICS_EXPORT HullShader : public Shader
{
 public:
  /**
  * Virtual destructor.
  */
  virtual
  ~HullShader() {};
};

}