#pragma once
#include "dr_input_layout.h"
#include <wrl.h>
#include <wrl/client.h>
class ID3D11InputLayout;
class Shader;
namespace driderSDK {
class D3DInputLayout : public InputLayout
{
public:
  DR_GRAPHICS_ERROR::E create(const Device & device, const DrInputElementDesc* inputDescArray, UInt32 arraySize) override;
  void set(const DeviceContext& deviceContext) override;
  void release() override;
  Microsoft::WRL::ComPtr<ID3D11InputLayout> APILayout;
    
};
}