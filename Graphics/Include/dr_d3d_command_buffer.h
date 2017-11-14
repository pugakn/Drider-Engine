#pragma once
#include "dr_command_buffer.h"

class ID3D11Buffer;

namespace driderSDK {

class DeviceContext;

class DR_GRAPHICS_EXPORT D3DCommandBuffer : public CommandBuffer
{
 public:
  /**
  * TEST::constructor
  *
  * Default constructor.
  */
  D3DCommandBuffer();

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
  DR_GRAPHICS_ERROR::E
  create(const Device& device, const DrBufferDesc& desc, const byte* initialData) override;
  //void set(const DeviceContext& deviceContext, DR_SHADER_TYPE_FLAG::E typeFlag) const override;

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
  updateFromBuffer(const DeviceContext& deviceContext, const byte* dataBuffer) override;

  /**
  * TEST::release
  * Release the allocated memory
  */
  void
  release() override;

  ID3D11Buffer* CB;
};

}