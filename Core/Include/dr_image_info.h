#pragma once

#include <vector>
#include "dr_core_prerequisites.h"

namespace driderSDK {

struct DR_CORE_EXPORT ImageInfo
{
  UInt32 width;
  UInt32 height;
  UInt32 channels;
  std::vector<UInt8> data;
};

}
