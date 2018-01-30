#pragma once
#include "dr_sound_prerequisites.h"

namespace driderSDK {

class SoundSystem;
class DrChannel;

class DR_SOUND_EXPORT DrSound {
  public:
    //DrSound() {}
    virtual ~DrSound() {}
  
    virtual void
    init(SoundSystem *system,
         DrChannel *channel) = 0;

    virtual void*
    getReference() = 0;

    virtual void**
    getObjectReference() = 0;

    virtual void
    play() = 0;

    virtual DR_SOUND_RESULT::E
    setMode(DR_SOUND_MODE::E mode) = 0;


};

}