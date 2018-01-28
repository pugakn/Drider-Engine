#pragma once
#include "dr_sound_prerequisites.h"
#include <memory>
#include "dr_sound_defines.h"

namespace driderSDK {

class SoundSystem;

class DR_SOUND_EXPORT SoundSystemManager {
 public:
  SoundSystemManager() {};
  ~SoundSystemManager() {};
  
  static SoundSystem*
  createSystem();
  
};
}