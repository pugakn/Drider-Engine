#include "dr_fmod_sound_api.h"

#include "dr_fmod_channel.h"
#include "dr_fmod_sound.h"
#include "dr_fmod_soundSystem.h"

namespace driderSDK {

void
FMODSoundAPI::init() {

  system = new FMODSoundSystem;
  
  system->init(32, DR_INITFLAGS::kDrInitFlags_NORMAL, 0);
}

void
FMODSoundAPI::update() {
  system->update();
}

void
FMODSoundAPI::destroy() {

}


}