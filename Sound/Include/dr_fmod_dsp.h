#pragma once
#include "dr_dsp.h"
#include <FMOD\fmod.hpp>

namespace driderSDK {

class DR_SOUND_EXPORT FMODDSP : public DrDSP {

  public:
    FMODDSP() {}
    virtual ~FMODDSP() {}

    void*
    getReference() override;

    void**
    getObjectReference() override;

    void
    setBypass(bool bypass) override;

    void
    getBypass(bool *bypass) override;

  private:
    FMOD::DSP *dsp;
    FMOD_RESULT result;
};

}