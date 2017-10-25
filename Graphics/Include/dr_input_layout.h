#pragma once
#include <dr_prerequisites.h>
#include "dr_graphics_defines.h"
namespace driderSDK {
class Device;
class DeviceContext;
class DR_API_EXPORT InputLayout
{
public:
  virtual ~InputLayout(){}
  virtual DR_GRAPHICS_ERROR::E create(const Device & device, const DrInputElementDesc* inputDescArray, UInt32 arraySize) = 0;
  virtual void set(const DeviceContext& deviceContext)const = 0;
  virtual void release() = 0;
};
}