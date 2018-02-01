#include "dr_sound_core.h"
#include "dr_sound_info.h"

#include <dr_fmod_sound.h>
#include <dr_soundSystem.h>
#include <dr_channel.h>

namespace driderSDK {

void
SoundCore::init(void* pData) {
  SoundInfo *info = static_cast<SoundInfo*>(pData);
  name = info->name;
  auto system = reinterpret_cast<SoundSystem*>(info->soundSystem);  

  soundResource = new FMODSound;
  system->createSound(name,
                      DR_SOUND_MODE::kDrMode_DEFAULT,
                      0,
                      soundResource);
}

DrSound *
SoundCore::get() {
  return soundResource;
}

}