#pragma once
#include "dr_compute_shader.h"

class ID3D11ComputeShader;

namespace driderSDK {

class DR_API_EXPORT D3DComputeShader : public ComputeShader
{
 public:
  void
  set(const DeviceContext& deviceContext) const override;

  void
  release() override;

  DR_GRAPHICS_ERROR::E
  createFromMemory(const Device& device, const char* buffer, size_t bufferSize) override;

  ID3D11ComputeShader* APIShader;
};

}