#include "dr_fmod_sound.h"

namespace driderSDK {

void*
FMODSound::getReference() {
  return fmodSound;
}

void**
FMODSound::getObjectReference() {
  return reinterpret_cast<void**>(&fmodSound);
}

DR_SOUND_RESULT::E
FMODSound::setMode(DR_SOUND_MODE::E mode) {
  result = fmodSound->setMode(static_cast<FMOD_MODE>(mode));
  return static_cast<DR_SOUND_RESULT::E>(result);
}

}