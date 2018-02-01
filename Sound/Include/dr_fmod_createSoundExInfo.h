#pragma once
#include "dr_sound_prerequisites.h"
#include "dr_createSoundExInfo.h"

#include <FMOD\fmod.hpp>

namespace driderSDK {

class DR_SOUND_EXPORT FMODCreateSoundExInfo : public DrCreateSoundExInfo{
  public:
    FMODCreateSoundExInfo() {}
    virtual ~FMODCreateSoundExInfo() {}
    
   void*
   getReference() override;

  public:
    FMOD_CREATESOUNDEXINFO *createSoundExInfo;
};

}