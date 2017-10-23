#pragma once
#include <dr_prerequisites.h>
#include "dr_graphics_defines.h"
namespace driderSDK {
class Device;
class DeviceContext;
class DR_API_EXPORT Shader
{
public:
  virtual ~Shader() {};
  virtual void set(const DeviceContext& deviceContext) const = 0;
  virtual void get (const DeviceContext& deviceContext) = 0;
  virtual DR_GRAPHICS_ERROR::E createFromMemory(const Device& device,const char* buffer, size_t bufferSize) = 0;
};
}