#include "dr_fmod_soundSystem.h"

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

DR_SOUND_RESULT::E
FMODSoundSystem::update() {
  result = fmodSoundSystem->update();

  return static_cast<DR_SOUND_RESULT::E>(result);
}

void*
FMODSoundSystem::getReference() {
  return fmodSoundSystem;
}

void**
FMODSoundSystem::getObjectReference() {
  return reinterpret_cast<void**>(&fmodSoundSystem);
}

DR_SOUND_RESULT::E 
FMODSoundSystem::createSound(TString name,
                             DR_SOUND_MODE::E mode,
                             DrCreateSoundExInfo* createInfo,
                             DrSound* sound) {
  result = fmodSoundSystem->createSound((const char*)name.c_str(),
                                        mode,
                                        0,
                                        reinterpret_cast<FMOD::Sound**>
                                          (sound->getObjectReference()));

  return static_cast<DR_SOUND_RESULT::E>(result);
}

//DR_SOUND_RESULT::E
//FMODSoundSystem::playSound(DrSound* sound,
//                           DrChannelGroup* channelgroup,
//                           bool paused,
//                           DrChannel* channel) {
//
//  result = fmodSoundSystem->playSound(reinterpret_cast<FMOD::Sound*>
//                                        (sound->getReference()),
//                                      0,
//                                      paused,
//                                      reinterpret_cast<FMOD::Channel**>
//                                        (channel->getObjectReference()));
//
//  return static_cast<DR_SOUND_RESULT::E>(result);
//}

}