#pragma once
#include "dr_sound_prerequisites.h"
#include <dr_module.h>

namespace driderSDK {

class DrSound;
class DrChannel;
class SoundSystem;
  
class DR_SOUND_EXPORT SoundAPI  : public Module<SoundAPI> {

  public:
    virtual void
    init() = 0;

    virtual void
    update() = 0;

    virtual void
    destroy() = 0;

    SoundSystem *system;
};

}