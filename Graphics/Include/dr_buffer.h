#pragma once
#include "dr_graphics_prerequisites.h"
#include "dr_graphics_defines.h"
#include <vector>

namespace driderSDK {

class Device;
class DeviceContext;

/**
* Base class for buffer shader resource
*
* Sample usage:
*	Buffer* = new D3DBuffer;
*/
class DR_GRAPHICS_EXPORT Buffer
{
 public:
   virtual void*
     getAPIObject() = 0;

   virtual void**
     getAPIObjectReference() = 0;

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
  virtual void
  create(const Device& device, const DrBufferDesc& desc, const byte* initialData) = 0;

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
  updateFromSysMemCpy(const DeviceContext& deviceContext) = 0;

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
    updateFromBuffer(const DeviceContext& deviceContext,
      const byte* buffer) = 0;

  /**
  * Release the allocated memory
  */
  virtual void
  release() = 0;

  std::vector<byte> m_sysMemCpy;
  DrBufferDesc m_descriptor;
};

}