#pragma once
#include "dr_vertex_shader.h"

struct ID3D11VertexShader;

namespace driderSDK {

class DR_API_EXPORT D3DVertexShader : public VertexShader
{
 public:
  void
  set(const DeviceContext& deviceContext) const override;

  void
  release() override;

  DR_GRAPHICS_ERROR::E
  createFromMemory(const Device& device, const char* buffer, size_t bufferSize) override;

  ID3D11VertexShader* APIShader;
};

}