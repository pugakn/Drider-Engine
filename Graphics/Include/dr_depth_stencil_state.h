#pragma once
#include <dr_prerequisites.h>
#include "dr_graphics_defines.h"
namespace driderSDK {
class Device;
class DeviceContext;
class DR_API_EXPORT DepthStencilState {
public:
  virtual DR_GRAPHICS_ERROR::E create(const Device& device, const DrDepthStencilDesc& desc) = 0;
  virtual void set(const DeviceContext& deviceContext, UInt32 refValue)const = 0;
  virtual void release() = 0;
};
}