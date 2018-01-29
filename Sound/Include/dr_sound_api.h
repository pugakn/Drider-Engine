#pragma once
#include "dr_sound_prerequisites.h"

namespace driderSDK {

class DrSound;
class DrChannel;
class SoundSystem;
  
class DR_SOUND_EXPORT SoundAPI {

  public:
    virtual void
    init() = 0;

    virtual void
    update() = 0;

    virtual void
    play() = 0;

    virtual void
    destroy() = 0;

    SoundSystem *system;
    DrSound* sounds;
    DrChannel *channel;

};

}