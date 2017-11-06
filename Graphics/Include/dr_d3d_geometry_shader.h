#pragma once
#include "dr_geometry_shader.h"

class ID3D11GeometryShader;

namespace driderSDK {

class DR_API_EXPORT D3DGeometryShader : public GeometryShader
{
 public:
  void
  set(const DeviceContext& deviceContext) const override;

  void
  release() override;

  DR_GRAPHICS_ERROR::E
  createFromMemory(const Device& device, const char* buffer, size_t bufferSize) override;

  ID3D11GeometryShader* APIShader;
};

}