#include "dr_fmod_soundSystem.h"

#include <comdef.h> 

#include "dr_sound.h"
#include "dr_channel.h"
#include "dr_channelGroup.h"
#include "dr_createSoundExInfo.h"

namespace driderSDK {

DR_SOUND_RESULT::E
FMODSoundSystem::init(Int32 maxChannels,
                      DR_INITFLAGS::E initFlags,
                      void* extraDriverData) {
  
  result = FMOD::System_Create(&fmodSoundSystem);
  result = fmodSoundSystem->init(maxChannels,
                                 initFlags,
                                 extraDriverData);

  return static_cast<DR_SOUND_RESULT::E>(result);
}

void
FMODSoundSystem::update() {
  result = fmodSoundSystem->update();
}

void*
FMODSoundSystem::getReference() {
  return fmodSoundSystem;
}

void**
FMODSoundSystem::getObjectReference() {
  return reinterpret_cast<void**>(&fmodSoundSystem);
}

void
FMODSoundSystem::createSound(TString name,
                             DR_SOUND_MODE::E mode,
                             DrCreateSoundExInfo* createInfo,
                             DrSound* sound) {

  _bstr_t unicode_name(name.c_str());
  result = fmodSoundSystem->createSound(unicode_name,
                                        mode,
                                        0,
                                        reinterpret_cast<FMOD::Sound**>
                                          (sound->getObjectReference()));
}

}