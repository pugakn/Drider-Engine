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
  
  void*
  getReference();

  private:
  FMOD::Sound *fmodSound;
};

}