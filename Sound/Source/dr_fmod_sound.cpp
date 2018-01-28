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
  result = fmodSound->setMode(FMOD_LOOP_OFF);
  return static_cast<DR_SOUND_RESULT::E>(result);
}

}