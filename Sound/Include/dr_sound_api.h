#pragma once
#include "dr_sound_prerequisites.h"
#include <dr_module.h>
#include "dr_sound_defines.h"

namespace driderSDK {

class DrSound;
class DrChannel;
class DrChannelGroup;
class SoundSystem;
class DrDSP;
class FMODSoundAPI;
  
class DR_SOUND_EXPORT SoundAPI  : public Module<SoundAPI> {

  public:
    SoundAPI();

    ~SoundAPI();
    
    FMODSoundAPI *API;

  private:
    void
    onStartUp() override;
  
    void
    onShutDown() override;
};

}