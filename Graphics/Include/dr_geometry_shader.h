#pragma once
#include "dr_shader.h"

namespace driderSDK {

/**
* Base class for geometry shader
*
* Sample usage:
*	Geometryshader* = new D3DGeometryShader;
*/
class DR_GRAPHICS_EXPORT GeometryShader : public Shader
{
 public:
  /**
  * Virtual destructor.
  */
  virtual
  ~GeometryShader() {};
};

}