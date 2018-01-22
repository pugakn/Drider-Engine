#pragma once
#include "dr_sound_prerequisites.h"

#include "dr_soundSystem.h"
#include "dr_sound_defines.h"
#include <FMOD\fmod.hpp>

namespace driderSDK {

class DR_SOUND_EXPORT FMODSoundSystem : public SoundSystem {
 public:
  void init(Int32 maxChannels,
            DR_INITFLAGS::E initFlags,
            void *extraDriverData);

 public:
  FMOD::System *fmodSoundSystem;
};

}