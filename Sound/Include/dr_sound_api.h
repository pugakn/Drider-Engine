#pragma once
#include "dr_sound_prerequisites.h"
#include <../../Core/Include/dr_module.h>

namespace driderSDK {

class DrSound;
class DrChannel;
class DrChannelGroup;
class SoundSystem;
class DrDSP;
  
class DR_SOUND_EXPORT SoundAPI  : public Module<SoundAPI> {

  public:
    virtual void
    init() = 0;

    virtual void
    update() = 0;

    virtual void
    destroy() = 0;

    SoundSystem *system;

    DrChannel* channel1;
    DrChannel* channel2;
    DrChannel* channel3;
    DrChannel* channel4;

    DrChannelGroup* masterGroup;
    DrDSP *dspLowPass;
};

}