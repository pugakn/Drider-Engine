#pragma once
#include "dr_shader.h"

namespace driderSDK {

/**
* Base class for pixel shader
*
* Sample usage:
*	FragmentShader* = new D3DFragmentShader;
*/
class DR_API_EXPORT FragmentShader : public Shader
{
 public:
   /**
   * Virtual destructor.
   */
  virtual
  ~FragmentShader() {};
};

}