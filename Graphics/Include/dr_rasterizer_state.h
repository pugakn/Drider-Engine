#pragma once
#include <dr_prerequisites.h>
#include "dr_graphics_defines.h"
namespace driderSDK {
class DeviceContext;
class Device;
class DR_API_EXPORT RasterizerState
{
 public:
   virtual ~RasterizerState() {}
   virtual DR_GRAPHICS_ERROR::E create(const Device& device,const DrRasterizerDesc& desc) = 0;
   virtual void set(const DeviceContext& deviceContext)const = 0;
   virtual void release() = 0;
};
}