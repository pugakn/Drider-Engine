#pragma once
#include "dr_shader.h"

namespace driderSDK {

/**
* Base class for texture shader
*
* Sample usage:
*	TextureShader* = new D3DTextureShader;
*/
class DR_API_EXPORT TextureShader : public Shader
{
 public:
  /**
  * Virtual destructor.
  */
  virtual
  ~TextureShader() {}
};

}