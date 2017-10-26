#pragma once
#include "sr_texture_shader.h"
#include <wrl.h>
#include <wrl/client.h>
#include <d3d11.h>
#include <dxgi.h>

namespace driderSDK {

class DR_API_EXPORT D3DTextreShader : public TextureShader
{
 public:
  void
  set(const DeviceContext& deviceContext) const override;

  void
  release() override;

  DR_GRAPHICS_ERROR::E
  createFromMemory(const Device& device, const char* buffer, size_t bufferSize) override;
  //Microsoft::WRL::ComPtr<ID3D11TextureShader> APIShader;
};

}