#pragma once
#include "dr_sound_prerequisites.h"

namespace driderSDK {

class DR_SOUND_EXPORT DrChannel {
  public:
    DrChannel() {}
    virtual ~DrChannel() {}

    virtual void*
    getReference() = 0;

};

}