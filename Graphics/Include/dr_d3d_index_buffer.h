#pragma once
#include "dr_index_buffer.h"
#include <wrl.h>
#include <wrl/client.h>

struct ID3D11Buffer;

namespace driderSDK {

class DeviceContext;

/**
* Class for index buffer shader resource
*
* Sample usage:
*	IndexBuffer* = new D3DIndexBuffer;
*/
class DR_GRAPHICS_EXPORT D3DIndexBuffer : public IndexBuffer
{
 public:
   void*
   getAPIObject() override;

   void**
   getAPIObjectReference() override;

  /**
  * TEST::constructor
  *
  * Default constructor.
  */
  D3DIndexBuffer();

  /**
  * TEST::create
  *
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
  void
  create(const Device& device,
         const DrBufferDesc& desc,
         const byte* initialData) override;

  /**
  * TEST::set
  *
  * Set the index buffer to the stage
  *
  * @param deviceContext
  *   The device context to set the resource
  *
  * @param offset
  *   The offset of the first element
  *
  */
  void
  set(const DeviceContext& deviceContext, UInt32 offset) const override;

  /**
  * TEST::updateFromMemory
  *
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
  void
  updateFromSysMemCpy(const DeviceContext& deviceContext) override;

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
  void
  updateFromBuffer(const DeviceContext& deviceContext,
                   const byte* dataBuffer) override;

  /**
  * TEST::release
  *
  * Release the allocated memory
  */
  void
  release() override;

  ID3D11Buffer* IB;
};

}