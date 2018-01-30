#pragma once
#include "dr_shader.h"
namespace driderSDK {
  class DR_GRAPHICS_EXPORT D3DShader : public Shader {
  public:
    std::vector<DrInputElementDesc>
      reflect() override;
  };
}