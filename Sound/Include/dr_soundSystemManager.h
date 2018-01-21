#pragma once
#include "dr_sound_prerequisites.h"

namespace driderSDK {

  class APISound;

  class DR_SOUND_EXPORT SoundSystemManager {
   public:
    SoundSystemManager() {};
    ~SoundSystemManager() {};
    
    static void systemCreate(APISound ** apiSound);
  };
}