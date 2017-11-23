#pragma once
#include "dr_graphics_prerequisites.h"
#include "dr_graphics_defines.h"

namespace driderSDK {

class Device;
class DeviceContext;

/**
* Base class for shaders
*
* Sample usage:
*	Shader* = new D3DFragmentShader;
*/
class DR_GRAPHICS_EXPORT Shader
{
 public:
  /**
  * Virtual destructor.
  */
  virtual
  ~Shader() {};

  /**
  * Set the shader to the stage
  *
  * @param deviceContext
  *   The device context to set the shader
  *
  */
  virtual void
  set(const DeviceContext& deviceContext) const = 0;

  /**
  * Release the allocated memory
  */
  virtual void
  release() = 0;

  /**
  * Update the buffer with new data
  *
  * @param deviceContext
  *   The device context to get acces to the resource
  *
  * @param buffer
  *   The uncompiled shader buffer
  *
  * @param bufferSize
  *   The uncompiled shader buffer size
  *
  * @return
  *   Return a DR_GRAPHICS_ERROR code, ERROR_NONE means all went well
  */
  virtual DR_GRAPHICS_ERROR::E
  createFromMemory(const Device& device, const char* buffer, size_t bufferSize) = 0;
};

}