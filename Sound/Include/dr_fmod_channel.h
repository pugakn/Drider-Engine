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
    
  private:
    FMOD::Channel *channel;
};

}