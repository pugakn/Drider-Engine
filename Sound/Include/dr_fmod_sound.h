#pragma once
#include "dr_sound_prerequisites.h"

#include "dr_sound_defines.h"
#include "dr_sound.h"

#include <FMOD\fmod.hpp>

namespace driderSDK {

class DR_SOUND_EXPORT FMODSound : public DrSound {
  public:
    FMODSound() {}
    virtual ~FMODSound() {}
    
    void
    init(SoundSystem *system,
         DrChannel *channel) override;

    void*
    getReference() override;

    virtual void**
    getObjectReference() override;

    DR_SOUND_RESULT::E
    setMode(DR_SOUND_MODE::E mode) override;

    void
    play() override;
    
  private:
    FMOD::Sound *fmodSound;
    FMOD::Channel *m_Channel;
    FMOD::System *m_System;
    FMOD_RESULT result;
};

}