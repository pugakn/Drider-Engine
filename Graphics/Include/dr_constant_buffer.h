#pragma once

#include "dr_buffer.h"

namespace driderSDK {

class DeviceContext;

/**
* Base class for constant buffer shader resource
*
* Sample usage:
*	ConstantBuffer* = new D3DConstantBuffer;
*/
class DR_GRAPHICS_EXPORT ConstantBuffer : public Buffer
{
 public:
  /**
  * Virtual destructor.
  */
  virtual
  ~ConstantBuffer() {}

  /**
  * Set the constant buffer to all shaders specifyed on typeFlag
  *
  * @param deviceContext
  *   The device context to set the resource
  *
  * @param typeFlag
  *   Bit flag that specifyes the shaders to set the constant buffer
  *
  */
  virtual void
  set(const DeviceContext& deviceContext, Int32 typeFlag = 0, Int32 startSlot = 0) const = 0;
};

}