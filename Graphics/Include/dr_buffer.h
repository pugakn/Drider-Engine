#pragma once
#include <dr_prerequisites.h>
#include "dr_graphics_defines.h"

namespace driderSDK {

class Device;
class DeviceContext;

/**
* Base class for buffer shader resource
*
* Sample usage:
*	Buffer* = new D3DBuffer;
*/
class DR_API_EXPORT Buffer
{
 public:
  /**
  * Virtual destructor.
  */
  virtual
  ~Buffer() {};

  /**
  * Create a buffer shader resource
  *
  * @param device
  *   The device to create the buffer
  *
  * @param desc
  *   The descriptor of the buffer
  *
  * @param initialData
  *   Initial data to fill the buffer
  *
  * @return
  *   Return a DR_GRAPHICS_ERROR code, ERROR_NONE means all went well
  */
  virtual DR_GRAPHICS_ERROR::E
  create(const Device& device, const DrBufferDesc& desc, char* initialData) = 0;

  /**
  * Update the buffer with new data
  *
  * @param deviceContext
  *   The device context to get acces to the resource
  *
  * @param dataBuffer
  *   The new data buffer
  *
  * @param bufferSize
  *   The new data buffer size
  */
  virtual void
  updateFromMemory(const DeviceContext& deviceContext, 
                   const char * dataBuffer, 
                   size_t bufferSize) = 0;

  /**
  * Release the allocated memory
  */
  virtual void
  release() = 0;

  DrBufferDesc descriptor;
};

}