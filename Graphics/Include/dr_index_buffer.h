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
class DR_API_EXPORT IndexBuffer : public Buffer
{
public:
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
  set(const DeviceContext& deviceContext, UInt32 offset) const = 0;
};
}