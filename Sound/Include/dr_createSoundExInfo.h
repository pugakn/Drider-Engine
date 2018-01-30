#pragma once
#include "dr_sound_prerequisites.h"

namespace driderSDK {

class DR_SOUND_EXPORT DrCreateSoundExInfo {
  public:
    DrCreateSoundExInfo(){}
    virtual ~DrCreateSoundExInfo() {}

    virtual void*
    getReference() = 0;
};

}