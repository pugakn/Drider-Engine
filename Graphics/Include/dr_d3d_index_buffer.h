#pragma once
#include "dr_index_buffer.h"
#include <wrl.h>
#include <wrl/client.h>

class ID3D11Buffer;

namespace driderSDK {

class DeviceContext;

/**
* Class for index buffer shader resource
*
* Sample usage:
*	IndexBuffer* = new D3DIndexBuffer;
*/
class DR_API_EXPORT D3DIndexBuffer : public IndexBuffer
{
 public:
  D3DIndexBuffer();

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
  DR_GRAPHICS_ERROR::E
  create(const Device& device, const DrBufferDesc& desc, char* initialData) override;

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
  void
  set(const DeviceContext& deviceContext, UInt32 offset) const override;

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
  updateFromMemory(const DeviceContext& deviceContext,
                   const char* dataBuffer,
                   size_t bufferSize) override;

  /**
  * Release the allocated memory
  */
  void
  release() override;

  Microsoft::WRL::ComPtr<ID3D11Buffer> IB;
};

}