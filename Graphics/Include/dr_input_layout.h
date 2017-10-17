#pragma once
#include <dr_prerequisites.h>
#include "dr_graphics_defines.h"
namespace driderSDK {

struct DrInputDesc 
{
  UInt32 semanticIndex;
  DR_FORMAT::E format;
  UInt32 offset;
};
class InputLayout 
{
  DrInputDesc inputDesc;
};
}