#pragma once
#include "dr_constant_buffer.h"

class ID3D11Buffer;

namespace driderSDK {

class DeviceContext;

class DR_API_EXPORT D3DConstantBuffer : public ConstantBuffer
{
 public:
  /**
  * Class default constructor.
  */
  D3DConstantBuffer();

  DR_GRAPHICS_ERROR::E
  create(const Device& device,
         const DrBufferDesc& desc,
         char* initialData) override;

  void
  set(const DeviceContext& deviceContext,
      DR_SHADER_TYPE_FLAG::E typeFlag) const override;

  void
  updateFromMemory(const DeviceContext& deviceContext,
                   const char * dataBuffer,
                   size_t bufferSize) override;

  void
  release() override;

  ID3D11Buffer* CB;
};

}