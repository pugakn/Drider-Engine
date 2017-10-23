#pragma once
#include <dr_prerequisites.h>
#include "dr_graphics_defines.h"
#include <string>

namespace driderSDK {
class SamplerState;
class DR_API_EXPORT Texture {
 public:
  virtual ~Texture(){}
  virtual DR_GRAPHICS_ERROR::E create(DR_FORMAT::E format, UInt32 w, UInt32 h) = 0;
  virtual DR_GRAPHICS_ERROR::E map(char* buffer) = 0;
  virtual void set() const = 0;
  virtual void unmap() = 0;
  virtual void udpateFromMemory(const char* buffer, size_t bufferSize) = 0;
  virtual void generateMipMaps() = 0;

  DR_FORMAT::E format;
  DrTextureDesc desc;
 private:
};
}