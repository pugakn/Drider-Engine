#pragma once
#include "dr_indirect_args_buffer.h"

struct ID3D11Buffer;
struct ID3D11UnorderedAccessView;
struct ID3D11ShaderResourceView;
namespace driderSDK {

  class DeviceContext;
  class Device;
  /**
  * Class for constant buffer shader resource
  *
  * Sample usage:
  *	StructureBuffer* = new D3DStructureBuffer;
  */
  class DR_GRAPHICS_EXPORT D3DIndirectArgsBuffer : public IndirectArgsBuffer
  {
  public:
    void*
      getAPIObject() const override;

    void**
      getAPIObjectReference() override;

    /**
    * TEST::
    *
    * Class default constructor.
    */
    D3DIndirectArgsBuffer();

    ~D3DIndirectArgsBuffer();

    /**
    * TEST::
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

    void
    set(const DeviceContext& deviceContext, Int32 typeFlag = 0, Int32 startSlot = 0) const override;

    /**
    * TEST::
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
    * TEST::
    *
    * Release the allocated memory
    */
    void
      release() override;

    ID3D11Buffer* m_buffer;
    ID3D11UnorderedAccessView * m_pBufferUAV;
  };

}