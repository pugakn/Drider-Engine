#pragma once
#include "dr_sound_prerequisites.h"

namespace driderSDK {
  
class DR_SOUND_EXPORT DrDSP {

  public:
    DrDSP() {}
    virtual ~DrDSP() {}

    virtual void*
    getReference() = 0;

    virtual void**
    getObjectReference() = 0;

    virtual void
    setBypass(bool bypass) = 0;

    virtual void
    getBypass(bool *bypass) = 0;

};

}