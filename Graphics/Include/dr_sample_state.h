#pragma once
#include <dr_prerequisites.h>
#include "dr_graphics_defines.h"
namespace driderSDK {
class Device;
class DeviceContext;
class DR_API_EXPORT SamplerState {
 public:
  virtual ~SamplerState() {}
  virtual DR_GRAPHICS_ERROR::E create(const Device& device, const DrSampleDesc& desc) = 0;
  virtual void set(const DeviceContext& deviceContext, DR_SHADER_TYPE_FLAG::E typeFlag) const = 0;
  virtual void release() = 0;
};
}