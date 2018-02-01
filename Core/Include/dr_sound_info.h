#pragma once
#include "dr_core_prerequisites.h"

namespace driderSDK {

struct DR_CORE_EXPORT SoundInfo 
{
  SoundInfo() {}

  TString name;
  void * soundSystem;
  void * channel;
};

}