#pragma once
#include "dr_sound_prerequisites.h"
#include "dr_channelGroup.h"

#include <memory>
#include <FMOD\fmod.hpp>

namespace driderSDK {
  class DR_SOUND_EXPORT FMODChannelGroup : public DrChannelGroup {
    public:
      FMODChannelGroup() {}
      virtual ~FMODChannelGroup() {}

      void * 
      getReference() override;

      void **
      getObjectReference() override;

      DrChannelGroup*
      get() override;

      void
      addDSP(Int32 index,
             DrDSP * dsp) override;

      private:
      FMOD::ChannelGroup *channelGroup;
      FMOD_RESULT result;
  };
}