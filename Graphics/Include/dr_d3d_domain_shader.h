#pragma once
#include "dr_domain_shader.h"
#include <wrl.h>
#include <wrl/client.h>

class ID3D11DomainShader;

namespace driderSDK {

class DR_API_EXPORT D3DDomainShader : public DomainShader
{
 public:
  void
  set(const DeviceContext& deviceContext) const override;

  void
  release() override;

  DR_GRAPHICS_ERROR::E
  createFromMemory(const Device& device, const char* buffer, size_t bufferSize) override;

  Microsoft::WRL::ComPtr<ID3D11DomainShader> APIShader;
};

}