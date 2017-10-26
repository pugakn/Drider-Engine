#pragma once
#include "dr_input_layout.h"
#include <wrl.h>
#include <wrl/client.h>
struct ID3D11InputLayout;
namespace driderSDK {
class DR_API_EXPORT D3DInputLayout : public InputLayout
{
public:
  DR_GRAPHICS_ERROR::E create(const Device & device, const DrInputElementDesc* inputDescArray, UInt32 arraySize) override;
  void set(const DeviceContext& deviceContext)const override;
  void release() override;
  Microsoft::WRL::ComPtr<ID3D11InputLayout> APILayout;
    
};
}