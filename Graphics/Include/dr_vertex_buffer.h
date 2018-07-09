#pragma once
#include "dr_buffer.h"
#include <vector>
namespace driderSDK {

class DeviceContext;

/**
* Base class for vertex buffer shader resource
*
* Sample usage:
*	VertexBuffer* = new D3DVertexBuffer;
*/
class DR_GRAPHICS_EXPORT VertexBuffer : public Buffer
{
 public:
  /**
  * Virtual destructor.
  */
  virtual
  ~VertexBuffer() {}

  /**
  * Set the vertex buffer to the stage
  *
  * @param deviceContext
  *   The device context to set the resource
  *
  * @param stride
  *   The size of one vertex data in bytes
  *
  * @param offset
  *   The offset of the first element
  *
  */
  virtual void
  set(const DeviceContext& deviceContext, UInt32 offset = 0)const = 0;

  virtual void
    set(const DeviceContext& deviceContext, VertexBuffer* extraBuffers, UInt32 offset = 0)const = 0;
};

}