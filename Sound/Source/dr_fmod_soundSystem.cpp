#include "dr_fmod_soundSystem.h"

namespace driderSDK {

  void FMODSoundSystem::init(Int32 maxChannels,
                             DR_INITFLAGS::E initFlags,
                             void *extraDriverData){
    
    FMOD::System_Create(&fmodSoundSystem);

    fmodSoundSystem->init(maxChannels, 
                          initFlags, 
                          extraDriverData);
  }
}