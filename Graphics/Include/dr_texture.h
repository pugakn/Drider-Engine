#pragma once
#include <dr_prerequisites.h>
#include "dr_graphics_defines.h"
#include <string>
namespace driderSDK {

class Texture {
 public:
  DR_FORMAT::E format;
  Int32 width;
  Int32 height;
  Int32 pitch;
  Int32 size;
  UInt32 id;
 private:
};
}