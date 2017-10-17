#pragma once
#include <dr_prerequisites.h>
#include "dr_graphics_defines.h"
#include <string>
namespace driderSDK {

class DR_API_EXPORT Texture {
 public:
  virtual void setTextures(UInt32 slot, UInt32 numTextures, Texture* texture) = 0;
  virtual void setSamplers(UInt32 slot, UInt32 numSamplers, SamplerState* texture) = 0;
  DR_FORMAT::E format;
  Int32 width;
  Int32 height;
  Int32 pitch;
  Int32 size;
  UInt32 id;
 private:
};
}