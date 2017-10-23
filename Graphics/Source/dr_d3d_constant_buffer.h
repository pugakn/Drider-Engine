#pragma once
#include "dr_constant_buffer.h"
#include <wrl.h>
#include <wrl/client.h>
class ID3D11Buffer;
namespace driderSDK {
class DeviceContext;
class DR_API_EXPORT D3DConstantBuffer : public ConstantBuffer
{
public:
  D3DConstantBuffer();
  ~D3DConstantBuffer();
  void create(const Device& device, const DrBufferDesc& desc, char* initialData) override;
  void set(const DeviceContext& deviceContext, UInt32 slot, UInt32 numBuffers, DR_SHADER_TYPE_FLAG::E typeFlag) const override;
  Microsoft::WRL::ComPtr<ID3D11Buffer> CB;
};
}