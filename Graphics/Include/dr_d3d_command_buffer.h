#pragma once
#include "dr_command_buffer.h"
#include <wrl.h>
#include <wrl/client.h>
class ID3D11Buffer;
namespace driderSDK {
class DeviceContext;
class DR_API_EXPORT D3DCommandBuffer : public CommandBuffer
{
public:
  D3DCommandBuffer();
  DR_GRAPHICS_ERROR::E create(const Device& device, const DrBufferDesc& desc, char* initialData) override;
  //void set(const DeviceContext& deviceContext, DR_SHADER_TYPE_FLAG::E typeFlag) const override;
  void release() override;
  Microsoft::WRL::ComPtr<ID3D11Buffer> CB;
};
}