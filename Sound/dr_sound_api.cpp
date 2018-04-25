#include "dr_sound_api.h"

#include "dr_fmod_sound_api.h"

namespace driderSDK {

SoundAPI::SoundAPI() {

}

SoundAPI::~SoundAPI() {

}

void
SoundAPI::onStartUp() {
  SoundAPI::instance().API = new FMODSoundAPI;
  SoundAPI::instance().API->init();
}

void
SoundAPI::onShutDown() {
}

}