#pragma once
#include "dr_buffer.h"

namespace driderSDK {

class DeviceContext;

/**
* Base class for index buffer shader resource
*
* Sample usage:
*	IndexBuffer* = new D3DIndexBuffer;
*/
class DR_GRAPHICS_EXPORT IndexBuffer : public Buffer
{
 public:
  /**
  * Virtual destructor.
  */
  virtual
  ~IndexBuffer() {}

  /**
  * Set the index buffer to the stage
  *
  * @param deviceContext
  *   The device context to set the resource
  *
  * @param offset
  *   The offset of the first element
  *
  */
  virtual void
  set(const DeviceContext& deviceContext,UInt32 offset = 0) const = 0;
};

}