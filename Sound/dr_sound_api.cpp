#include "dr_sound_api.h"

#include "dr_fmod_soundSystem.h"

namespace driderSDK {

SoundAPI::SoundAPI(Int32 channels,
                   DR_INITFLAGS::E initFlags,
                   void * extraDriverData) {

  system = new FMODSoundSystem();

  system->init(channels,
               initFlags,
               extraDriverData);
}

void
SoundAPI::update() {
  instance().update();
}

SoundAPI::~SoundAPI() {

}

void
SoundAPI::onStartUp() {
  system = new FMODSoundSystem();

  system->init(1,
               DR_INITFLAGS::kDrInitFlags_NORMAL,
               nullptr);
}

void
SoundAPI::onShutDown() {
  delete system;
}

}