#pragma once
#include <dr_prerequisites.h>
#include "dr_graphics_defines.h"
namespace driderSDK {
class Device;
class DeviceContext;
class DR_API_EXPORT SwapChain {
public:
  virtual ~SwapChain() {}
  virtual DR_GRAPHICS_ERROR::E create(const Device& device) = 0;
  virtual void release() = 0;
  virtual void swapBuffers() = 0;
};
}
