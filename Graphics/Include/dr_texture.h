#pragma once
#include <dr_prerequisites.h>
#include "dr_graphics_defines.h"
#include <string>
namespace driderSDK {

class Texture {
 public:
  Int32 width;
  Int32 height;
  Int32 pitch;
  Int32 size;
  UInt32 id;
  DR_FORMAT::E format;
  DR_TEXTURE_TILING::E tiling;
  std::string name;
 private:
};
}