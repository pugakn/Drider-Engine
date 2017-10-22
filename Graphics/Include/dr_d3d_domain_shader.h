#pragma once
#include "dr_domain_shader.h"
#include <wrl.h>
#include <wrl/client.h>
#include <d3d11.h>
#include <dxgi.h>
namespace driderSDK {
class DR_API_EXPORT D3DDomainShader : public DomainShader
{
public:
  void set(const DeviceContext& deviceContext) override;
  void createFromMemory(const Device& device, const char* buffer, size_t bufferSize) override;
  Microsoft::WRL::ComPtr<ID3D11DomainShader> APIShader;
};
}