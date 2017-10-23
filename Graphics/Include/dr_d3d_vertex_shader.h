#pragma once
#include "dr_vertex_shader.h"
#include <wrl.h>
#include <wrl/client.h>
class ID3D11VertexShader;
namespace driderSDK {
class DR_API_EXPORT D3DVertexShader : public VertexShader
{
public:
  void set(const DeviceContext& deviceContext) const override;
  void createFromMemory(const Device& device, const char* buffer, size_t bufferSize) override;
  Microsoft::WRL::ComPtr<ID3D11VertexShader> APIShader;
};
}