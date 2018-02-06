#include "dr_fmod_channelGroup.h"
#include "dr_fmod_dsp.h"

namespace driderSDK {

void *
FMODChannelGroup::getReference() {
  return channelGroup;
}

void **
FMODChannelGroup::getObjectReference() {
  return reinterpret_cast<void**>(&channelGroup);
}

void
FMODChannelGroup::addDSP(Int32 index,
                         DrDSP * dsp) {
  result = channelGroup->addDSP(index,
                       reinterpret_cast<FMOD::DSP*>(dsp));
}


  
}