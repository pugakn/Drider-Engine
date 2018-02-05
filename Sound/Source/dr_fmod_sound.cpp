#include "dr_fmod_sound.h"

namespace driderSDK {

void
FMODSound::init(SoundSystem *system,
                DrChannel **channel) {
  m_System = reinterpret_cast<FMOD::System*>(system);
  m_Channel = reinterpret_cast<FMOD::Channel**>(channel);
}

void
FMODSound::setSystem(SoundSystem* system) {
  m_System = reinterpret_cast<FMOD::System*>(system);
}

void
FMODSound::setChannel(DrChannel **channel){
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

void
FMODSound::setMode(DR_SOUND_MODE::E mode) {
  result = fmodSound->setMode(static_cast<FMOD_MODE>(mode));
}

void
FMODSound::play() {
  result = m_System->playSound(fmodSound, 0, false, m_Channel);
}

}