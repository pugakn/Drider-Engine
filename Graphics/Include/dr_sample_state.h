#pragma once
#include <dr_prerequisites.h>
#include "dr_graphics_defines.h"

namespace driderSDK {

class Device;
class DeviceContext;

class DR_API_EXPORT SamplerState
{
 public:
  /**
  * Class virutal destructor.
  */
  virtual 
  ~SamplerState() {}

  /**
  * Create a sample state
  *
  * @param device
  *   The device to create the state
  *
  * @param desc
  *   Describes the state params
  *
  * @return
  *   Return a DR_GRAPHICS_ERROR code, ERROR_NONE means all went well
  */
  virtual DR_GRAPHICS_ERROR::E
  create(const Device& device, const DrSampleDesc& desc) = 0;

  /**
  * Set the state
  *
  * @param deviceContext
  *   The device context to set the state
  *
  * @param typeFlag
  *   Specifyes the shaders to bind with a bitflag
  */
  virtual void
  set(const DeviceContext& deviceContext,
      DR_SHADER_TYPE_FLAG::E typeFlag) const = 0;

  /**
  * Release the allocated memory
  */
  virtual void
  release() = 0;
};

}