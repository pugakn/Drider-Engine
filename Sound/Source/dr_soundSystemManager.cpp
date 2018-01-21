#include "dr_soundSystemManager.h"

#include "dr_soundSystem.h"

namespace driderSDK {

  std::shared_ptr<SoundSystem>
  SoundSystemManager::createSystem(Int32 maxChannels,
                                   DR_INITFLAGS::E initFlags,
                                   void *extraDriverData) {
    std::shared_ptr<SoundSystem> soundSystem = 
    std::make_shared<SoundSystem>();
    
    soundSystem->init(maxChannels,
                      initFlags,
                      extraDriverData);

    return soundSystem;
  }
}