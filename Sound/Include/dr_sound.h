#pragma once
#include "dr_sound_prerequisites.h"

namespace driderSDK {

class DR_SOUND_EXPORT DrSound {
  public:
    DrSound() {}
    virtual ~DrSound() {}

    virtual void*
    getReference() = 0;


};

}