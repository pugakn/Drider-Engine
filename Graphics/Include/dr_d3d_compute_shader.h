#pragma once
#include "dr_compute_shader.h"
#include <wrl.h>
#include <wrl/client.h>
class ID3D11ComputeShader;
namespace driderSDK {
class DR_API_EXPORT D3DComputeShader : public ComputeShader
{
public:
  void set(const DeviceContext& deviceContext) const override;
  void createFromMemory(const Device& device, const char* buffer, size_t bufferSize) override;
  Microsoft::WRL::ComPtr<ID3D11ComputeShader> APIShader;
};
}