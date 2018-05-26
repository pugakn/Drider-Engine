#pragma once

#include <vector>

#include "dr_sound_prerequisites.h"
#include "dr_sound_api.h"

#include <dr_vector3d.h>

namespace driderSDK {

class DR_SOUND_EXPORT FMODSoundAPI : public SoundAPI {

  public:
    void
    init();

    void
    update();

    void
    destroy();

    /**
    * Select channel
    */
    void
    selectChannel(Int32 indexChannel);

    SoundSystem *system;

    DrChannel* channel1;
    DrChannel* channel2;
    DrChannel* channel3;
    DrChannel* channel4;

    DrChannelGroup* masterGroup;
    DrDSP *dspLowPass;

    Vector3D pos;

};

}