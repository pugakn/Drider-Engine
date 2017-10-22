#pragma once
#include "dr_constant_buffer.h"
#include <wrl.h>
#include <wrl/client.h>
#include <d3d11.h>
#include <dxgi.h>
namespace driderSDK {
  class DR_API_EXPORT D3DConstantBuffer : public ConstantBuffer
  {
  public:
    D3DConstantBuffer();
    ~D3DConstantBuffer();
    void create(const Device& device, const DrBufferDesc& desc, char* initialData) override;
    Microsoft::WRL::ComPtr<ID3D11Buffer> CB;
  };
}