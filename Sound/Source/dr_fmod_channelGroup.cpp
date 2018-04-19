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

DrChannelGroup*
FMODChannelGroup::get() {
  return reinterpret_cast<DrChannelGroup*>(channelGroup);
}

void
FMODChannelGroup::addDSP(Int32 index,
                         DrDSP * dsp) {
  result = channelGroup->addDSP(index,
                       reinterpret_cast<FMOD::DSP*>(dsp));
}


  
}