#pragma once
#include "dr_vertex_buffer.h"
#include <wrl.h>
#include <wrl/client.h>
#include <d3d11.h>
#include <dxgi.h>
namespace driderSDK {
class DR_API_EXPORT D3DVertexBuffer : public VertexBuffer
{
public:
  D3DVertexBuffer();
  ~D3DVertexBuffer();
  void create(const Device& device, const DrBufferDesc& desc, char* initialData) override;
  Microsoft::WRL::ComPtr<ID3D11Buffer> VB;
};
}


