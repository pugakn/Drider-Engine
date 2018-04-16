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
  
class DR_SOUND_EXPORT SoundAPI  : public Module<SoundAPI> {

  public:
    SoundAPI(Int32 channels,
             DR_INITFLAGS::E initFlags,
             void * extraDriverData);

    ~SoundAPI();

    static void 
    update();

    SoundSystem *system;

    DrChannel* channel1;
    DrChannel* channel2;
    DrChannel* channel3;
    DrChannel* channel4;

    DrChannelGroup* masterGroup;
    DrDSP *dspLowPass;

  private:
    void
    onStartUp() override;
  
    void
    onShutDown() override;
};

}