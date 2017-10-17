#pragma once
#include <dr_prerequisites.h>
#include "dr_graphics_defines.h"
namespace driderSDK {

struct BufferDesc 
{
  DR_BUFFER_USAGE::E usage;
  uint32_t stride;
  uint32_t sizeInBytes;
};

class Buffer 
{
 public:
  BufferDesc bufferDesc;
};
}