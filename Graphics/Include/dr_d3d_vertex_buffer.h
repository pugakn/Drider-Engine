#pragma once
#include "dr_vertex_buffer.h"
#include <wrl.h>
#include <wrl/client.h>
class ID3D11Buffer;
namespace driderSDK {
class DeviceContext;
class DR_API_EXPORT D3DVertexBuffer : public VertexBuffer
{
public:
  D3DVertexBuffer();
  DR_GRAPHICS_ERROR::E create(const Device& device, const DrBufferDesc& desc, char* initialData) override;
  void set(const DeviceContext& deviceContext, UInt32 stride, UInt32 offset) const override;
  void release() override;
  Microsoft::WRL::ComPtr<ID3D11Buffer> VB;
};
}


