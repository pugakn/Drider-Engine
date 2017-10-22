#pragma once
#include <dr_prerequisites.h>
#include "dr_texture.h"
#include "dr_sample_state.h"
#include "dr_constant_buffer.h"
namespace driderSDK {
class Device;
class DR_API_EXPORT Shader
{
public:
  virtual ~Shader() {};
  virtual void set() = 0;
  virtual void unset() = 0;
  virtual void createFromMemory(const Device& device,const char* buffer, size_t bufferSize) = 0;
  virtual void free() = 0;
};
}