#pragma once
#include "dr_index_buffer.h"
#include <wrl.h>
#include <wrl/client.h>
class ID3D11Buffer;
namespace driderSDK {
class DeviceContext;
class DR_API_EXPORT D3DIndexBuffer : public IndexBuffer
{
public:
  D3DIndexBuffer();
  DR_GRAPHICS_ERROR::E create(const Device& device, const DrBufferDesc& desc, char* initialData) override;
  void set(const DeviceContext& deviceContext, UInt32 offset) const override;
  void updateFromMemory(const DeviceContext& deviceContext, const char * dataBuffer, size_t bufferSize) override;
  void release() override;
  Microsoft::WRL::ComPtr<ID3D11Buffer> IB;
};
}