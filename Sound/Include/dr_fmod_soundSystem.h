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

   void
   playSound(DrSound* sound,
             DrChannelGroup* channelGroup,
             bool paused,
             DrChannel **channel) override;
    
   void
   set3DSettings(float dopplerscale,
                 float distancefactor,
                 float rolloffscale) override;

   void
   set3DListenerAttributes(Int32 listener,
                           const Vector3D* pos,
                           const Vector3D* vel,
                           const Vector3D* forward,
                           const Vector3D* up) override;

   void
   getMasterChannelGroup(DrChannelGroup** channelGroup) override;

   void
   createDSPByType(DR_DSP_TYPE::E bspType,
                   DrDSP **dsp) override;

   
  private:
    FMOD::System* fmodSoundSystem;
    FMOD_RESULT result;
};

}