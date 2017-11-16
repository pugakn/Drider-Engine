#pragma once
#include <dr_engine_prerequisites.h>
#include <vector>

namespace driderSDK {

struct DR_ENGINE_EXPORT sImage
{
  Int32 w;
  Int32 h;
  Int32 channels;
  //unsigned char data[];
  std::vector<unsigned char> data;
};
}