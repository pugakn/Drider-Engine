#pragma once
#include "dr_shader.h"

namespace driderSDK {

/**
* Base class for vertex shader
*
* Sample usage:
*	VertexShader* = new D3DVertexShader;
*/
class DR_GRAPHICS_EXPORT VertexShader : public Shader
{
 public:
  /**
  * Virtual destructor.
  */
  virtual
  ~VertexShader() {}
};

}