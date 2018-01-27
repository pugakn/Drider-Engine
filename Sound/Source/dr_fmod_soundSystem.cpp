#include "dr_fmod_soundSystem.h"

#include "dr_sound.h"
#include "dr_channel.h"
#include "dr_channelGroup.h"
#include "dr_createSoundExInfo.h"

namespace driderSDK {

void 
FMODSoundSystem::init(Int32 maxChannels,
                      DR_INITFLAGS::E initFlags,
                      void* extraDriverData) {

  FMOD::System_Create(&fmodSoundSystem);

  fmodSoundSystem->init(maxChannels, 
                        initFlags, 
                        extraDriverData);
}

/*void*
FMODSoundSystem::getReference() {
  return fmodSoundSystem;
}

DR_SOUND_RESULT::E 
FMODSoundSystem::createSound(const char *name,
                             DR_SOUND_MODE::E mode,
                             DrCreateSoundExInfo *createInfo,
                             DrSound ** sound) {

  FMOD_RESULT r;
  r = fmodSoundSystem->createSound(name,
                                   mode,
                                   reinterpret_cast<FMOD_CREATESOUNDEXINFO*>(createInfo->getReference()),
                                   reinterpret_cast<FMOD::Sound**> (sound));

  return static_cast<DR_SOUND_RESULT::E>(r);  
}

DR_SOUND_RESULT::E
FMODSoundSystem::playSound(DrSound *sound,
                           DrChannelGroup *channelgroup,
                           bool paused,
                           DrChannel *channel) {
  FMOD_RESULT r;
  r = fmodSoundSystem->playSound(reinterpret_cast<FMOD::Sound*>(sound->getReference()),
                                 reinterpret_cast<FMOD::ChannelGroup*>(channelgroup->getReference()),
                                 paused,
                                 reinterpret_cast<FMOD::Channel**>(channelgroup->getReference()));

  return static_cast<DR_SOUND_RESULT::E>(r);                      
}*/

}