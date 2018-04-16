#pragma once
#include "dr_sound_prerequisites.h"
#include "dr_sound_api.h"

namespace driderSDK {

class DR_SOUND_EXPORT FMODSoundAPI : public SoundAPI {

  public:
    void
    init();

    virtual void
    update();

    void
    destroy();


};

}