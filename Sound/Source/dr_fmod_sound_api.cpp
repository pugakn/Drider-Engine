#include "dr_fmod_sound_api.h"

#include "dr_fmod_channel.h"
#include "dr_fmod_sound.h"
#include "dr_fmod_soundSystem.h"

namespace driderSDK {

void
FMODSoundAPI::init() {

  system = new FMODSoundSystem;
  sound1 = new FMODSound;
  channel = new FMODChannel;
  
  system->init(32, DR_INITFLAGS::kDrInitFlags_NORMAL, 0);
  system->createSound("testSound.mp3", DR_SOUND_MODE::kDrMode_CREATESAMPLE, 0, sound1);
  sound1->setMode(DR_SOUND_MODE::kDrMode_LOOP_OFF);

}

void
FMODSoundAPI::play() {
  system->playSound(sound1, 0, false, channel);
}

void
FMODSoundAPI::update() {
  system->update();
}

void
FMODSoundAPI::destroy() {

}


}