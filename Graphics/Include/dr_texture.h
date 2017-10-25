#pragma once
#include <dr_prerequisites.h>
#include "dr_graphics_defines.h"
#include <string>

namespace driderSDK {
class Device;
class DeviceContext;
class DR_API_EXPORT Texture {
 public:
  virtual ~Texture(){}
  virtual DR_GRAPHICS_ERROR::E createFromMemory(const Device& device, const DrTextureDesc& desc, const char* buffer) = 0;
  virtual DR_GRAPHICS_ERROR::E createEmpty(const Device& device, const DrTextureDesc& desc) = 0;
  virtual DR_GRAPHICS_ERROR::E map(const DeviceContext& deviceContext,char* buffer) = 0;
  virtual void set(const DeviceContext& deviceContext, UInt32 slot) const = 0;
  virtual void release() = 0;
  virtual void unmap(const DeviceContext& deviceContext) = 0;
  virtual void udpateFromMemory(const DeviceContext& deviceContext, const char* buffer, size_t bufferSize) = 0;
  virtual void generateMipMaps(const DeviceContext & deviceContext) = 0;
 protected:
   DrTextureDesc descriptor;
};
}