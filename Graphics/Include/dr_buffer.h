#pragma once
#include <dr_prerequisites.h>
#include "dr_graphics_defines.h"

namespace driderSDK {
class Device;
class DR_API_EXPORT Buffer
{
 public:
  virtual ~Buffer() {};
  virtual void create(const Device& device, const DrBufferDesc& desc, char* initialData) = 0;
};
}