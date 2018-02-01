#include "dr_fmod_sound.h"

namespace driderSDK {

void
FMODSound::init(SoundSystem *system,
                DrChannel **channel) {
  m_System = reinterpret_cast<FMOD::System*>(system);
  m_Channel = reinterpret_cast<FMOD::Channel**>(channel);
}

FMOD::Sound *
FMODSound::get() {
  return fmodSound;
}

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

void
FMODSound::play() {
  result = m_System->playSound(fmodSound, 0, false, m_Channel);
}

}