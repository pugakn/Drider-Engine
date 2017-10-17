#pragma once
#include <dr_prerequisites.h>
#include "dr_texture.h"
#include "dr_sample_state.h"
#include "dr_constant_buffer.h"
namespace driderSDK {
class Shader
{
  virtual void setShaderTextures(UInt32 slot, UInt32 numTextures, Texture* texture) = 0;
  virtual void setConstantBuffers(UInt32 slot, UInt32 numBuffers, ConstantBuffer* buffer) = 0;
  virtual void setSamplers(UInt32 slot, UInt32 numSamplers, SamplerState* texture) = 0;
};
}