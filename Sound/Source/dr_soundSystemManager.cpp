#include "dr_soundSystemManager.h"

#include "dr_soundSystem.h"
#include "dr_fmod_soundSystem.h"

namespace driderSDK {

std::shared_ptr<SoundSystem>
SoundSystemManager::createSystem(Int32 maxChannels,
                                 DR_INITFLAGS::E initFlags,
                                 void *extraDriverData) {

  auto soundSystem = std::make_shared<FMODSoundSystem>();
  soundSystem->init(maxChannels, initFlags, extraDriverData);

  return soundSystem;
}
  
}