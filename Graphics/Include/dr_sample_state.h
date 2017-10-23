#pragma once
#include <dr_prerequisites.h>
#include "dr_graphics_defines.h"
namespace driderSDK {
class DR_API_EXPORT SamplerState {
 public:
  virtual ~SamplerState() {}
  virtual void set(UInt32 slot, UInt32 numSamplers) const = 0;
  DrSampleDesc sampleDesc;
};
}