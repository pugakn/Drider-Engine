#pragma once
#include <dr_engine_prerequisites.h>
#include <vector>

namespace driderSDK {

struct DR_ENGINE_EXPORT sImage
{
  Int32 w;
  Int32 h;
  Int32 channels;
  std::vector<unsigned char> data;
};
}