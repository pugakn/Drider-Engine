#pragma once
#include "dr_sound_prerequisites.h"
#include "dr_sound_api.h"

namespace driderSDK {

class DR_SOUND_EXPORT FMODSoundAPI : public SoundAPI {

  public:
    void
    init();

    void
    update();

    void
    destroy();

    SoundSystem *system;

    DrChannel* channel1;
    DrChannel* channel2;
    DrChannel* channel3;
    DrChannel* channel4;

    DrChannelGroup* masterGroup;
    DrDSP *dspLowPass;

};

}