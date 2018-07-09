#pragma once
#include "dr_sound_prerequisites.h"
#include "dr_channel.h"

#include <FMOD\fmod.hpp>

namespace driderSDK {

class DR_SOUND_EXPORT FMODChannel : public DrChannel {
  public:
    FMODChannel(){}
    virtual ~FMODChannel(){}
    
    void*
    getReference() override;

    void**
    getObjectReference() override;

    DrChannel*
    get() override;

    void
    setPaused(bool paused) override;

    void
    getPaused(bool *paused) override;

    void
    setVolume(float volume) override;

    void
    getVolume(float *volume) override;

    void
    setReverbProperties(Int32 instance,
                        float wet) override;

    void
    set3DAttributes(const Vector3D* pos,
                    const Vector3D* vel) override;
    
  private:
    FMOD::Channel *channel;
    FMOD_RESULT result;
};

}