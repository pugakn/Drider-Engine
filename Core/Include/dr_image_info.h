#pragma once

#include <vector>
#include <dr_graphics_defines.h>
#include "dr_core_prerequisites.h"

namespace driderSDK {

struct DR_CORE_EXPORT ImageInfo
{
  UInt32 width;
  UInt32 height;
  UInt32 channels;
  DR_DIMENSION::E textureDimension;
  std::vector<UInt8> data;
};

}
