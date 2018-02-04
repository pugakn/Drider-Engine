#include "dr_sound_core.h"
#include "dr_sound_info.h"

#include <dr_fmod_sound.h>
#include <dr_soundSystem.h>
#include <dr_channel.h>

namespace driderSDK {

void
SoundCore::init(void* pData) {
  
}

void
SoundCore::init(void* pData,
                void* extraData) {
  SoundInfo *info = static_cast<SoundInfo*>(pData);
  name = info->name;
  //auto system = reinterpret_cast<SoundSystem*>(info->soundSystem); 

  SoundExtraInfo *extraInfo = static_cast<SoundExtraInfo*>(extraData);

  soundResource = new FMODSound;
  soundResource->init(reinterpret_cast<SoundSystem*>(extraInfo->m_soundSystem->getReference()),
                      reinterpret_cast<DrChannel**>(extraInfo->m_channel->getObjectReference()));

  extraInfo->m_soundSystem->createSound(name,
                                        DR_SOUND_MODE::kDrMode_DEFAULT,
                                        0,
                                        soundResource);
}

DrSound *
SoundCore::get() {
  return soundResource;
}

}