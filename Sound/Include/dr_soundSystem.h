#pragma once
#include "dr_sound_prerequisites.h"
#include <memory>
#include <FMOD\fmod.hpp>

namespace driderSDK {

class DR_SOUND_EXPORT SoundSystem {
public:
  SoundSystem() {}
  virtual ~SoundSystem() {}

  virtual void init(Int32 maxChannels,
                    DR_INITFLAGS::E initFlags,
                    void *extraDriverData) = 0;
};

}