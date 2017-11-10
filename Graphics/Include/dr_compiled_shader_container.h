#pragma once
#include <dr_prerequisites.h>
#include "dr_graphics_defines.h"

namespace driderSDK {
class CompiledShaderContainer
{
 public:
   virtual void get() = 0;
   virtual void set() = 0;
};
}