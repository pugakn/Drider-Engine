#pragma once
#include "dr_sound_prerequisites.h"

#include "dr_soundSystem.h"
#include "dr_sound_defines.h"

#include <FMOD\fmod.hpp>

namespace driderSDK {

class DR_SOUND_EXPORT FMODSoundSystem : public SoundSystem {
  public:
   FMODSoundSystem() {}
   virtual ~FMODSoundSystem() {}

   virtual DR_SOUND_RESULT::E
   init(Int32 maxChannels,
        DR_INITFLAGS::E initFlags,
        void *extraDriverData) override;

   void
   update() override;

   void*
   getReference() override;

   void**
   getObjectReference() override;

   void 
   createSound(TString name,
               DR_SOUND_MODE::E mode,
               DrCreateSoundExInfo *createInfo,
               DrSound * sound) override;

    
   private:
     FMOD::System* fmodSoundSystem;
     FMOD_RESULT result;
};

}