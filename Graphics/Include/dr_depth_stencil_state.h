#pragma once
#include <dr_prerequisites.h>
#include "dr_graphics_defines.h"

namespace driderSDK {

class Device;
class DeviceContext;

class DR_API_EXPORT DepthStencilState
{
 public:
  /**
  * Create a detph stencil state that describes the behavior
  * of the deth stencil
  *
  * @param device
  *   The device to create the depth stencil state
  *
  * @param desc
  *   The descriptor of the depth stencil state
  *
  * @return
  *   Return a DR_GRAPHICS_ERROR code, ERROR_NONE means all went well
  */
  virtual DR_GRAPHICS_ERROR::E
  create(const Device& device, const DrDepthStencilDesc& desc) = 0;

  /**
  * Set the depth stencil state
  *
  * @param deviceContext
  *   The device context set the state
  *
  * @param refValue
  *   Ther reference value to perform against when doing a 
  *   depth-stencil test
  */
  virtual void
  set(const DeviceContext& deviceContext, UInt32 refValue) const = 0;

  /**
  * Release the allocated memory
  */
  virtual void
  release() = 0;
};

}