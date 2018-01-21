#include "dr_soundSystem.h"

#include "dr_soundSystemManager.h"
#include <FMOD\fmod.hpp>

namespace driderSDK {
  void
  SoundSystemManager::systemCreate(APISound ** apiSound) {
    
    FMOD::System *system;
    FMOD::System_Create(&system);

    
  }
}