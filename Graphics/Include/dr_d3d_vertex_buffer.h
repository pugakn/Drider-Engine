#pragma once
#include "dr_vertex_buffer.h"

struct ID3D11Buffer;

namespace driderSDK {

class DeviceContext;

/**
* Class for vertex buffer shader resource
*
* Sample usage:
*	VertexBuffer* = new D3DVertexBuffer;
*/
class DR_GRAPHICS_EXPORT D3DVertexBuffer : public VertexBuffer
{
 public:
  void*
  getAPIObject() const override;

  void**
  getAPIObjectReference() override;

  /**
  * TEST::constructor
  *
  * Default constructor.
  */
  D3DVertexBuffer();

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
  */
  void
  set(const DeviceContext& deviceContext,
      UInt32 offset) const override;

  void
    set(const DeviceContext& deviceContext, 
      VertexBuffer* extraBuffers, 
      UInt32 offset = 0)const override;

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

  ID3D11Buffer* VB;
};

}