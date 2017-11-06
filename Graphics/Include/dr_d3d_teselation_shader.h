#pragma once
#include "dr_teselation_shader.h"
#include <d3d11.h>
#include <dxgi.h>

namespace driderSDK {

class DR_API_EXPORT D3DTeselationShader : public TeselationShader
{
 public:
  void
  set(const DeviceContext& deviceContext) const override;

  void
  release() override;

  DR_GRAPHICS_ERROR::E
  createFromMemory(const Device& device, const char* buffer, size_t bufferSize) override;
  //Microsoft::WRL::ComPtr<ID3D11Tes> APIShader;
};

}