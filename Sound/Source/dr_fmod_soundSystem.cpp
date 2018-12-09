#include "dr_fmod_soundSystem.h"

//#include <comdef.h> 

#include "dr_sound.h"
#include "dr_channel.h"
#include "dr_channelGroup.h"
#include "dr_createSoundExInfo.h"

namespace driderSDK {

DR_SOUND_RESULT::E
FMODSoundSystem::init(Int32 maxChannels,
                      DR_INITFLAGS::E initFlags,
                      void* extraDriverData) {
  
  result = FMOD::System_Create(&fmodSoundSystem);
  result = fmodSoundSystem->init(maxChannels,
                                 initFlags,
                                 extraDriverData);

  return static_cast<DR_SOUND_RESULT::E>(result);
}

void
FMODSoundSystem::update() {
  result = fmodSoundSystem->update();
}

void*
FMODSoundSystem::getReference() {
  return fmodSoundSystem;
}

void**
FMODSoundSystem::getObjectReference() {
  return reinterpret_cast<void**>(&fmodSoundSystem);
}

SoundSystem*
FMODSoundSystem::get() {
  return reinterpret_cast<SoundSystem*>(fmodSoundSystem);
}

void
FMODSoundSystem::createSound(TString name,
                             DR_SOUND_MODE::E mode,
                             DrCreateSoundExInfo* createInfo,
                             DrSound* sound) {

  String vname = StringUtils::toString(name);
  result = fmodSoundSystem->createSound(vname.c_str(),
                                        mode,
                                        reinterpret_cast<FMOD_CREATESOUNDEXINFO*>
                                        (createInfo),
                                        reinterpret_cast<FMOD::Sound**>
                                        (sound->getObjectReference()));
}

void
FMODSoundSystem::playSound(DrSound* sound,
                           DrChannelGroup* channelGroup,
                           bool paused,
                           DrChannel **channel) {
  result = fmodSoundSystem->playSound(
  reinterpret_cast<FMOD::Sound*>(sound),
  reinterpret_cast<FMOD::ChannelGroup*>(channelGroup),
  paused,
  reinterpret_cast<FMOD::Channel**>(channel));
}

void
FMODSoundSystem::set3DSettings(float dopplerscale,
                               float distancefactor,
                               float rolloffscale) {
  result = fmodSoundSystem->set3DSettings(dopplerscale,
                                          distancefactor,
                                          rolloffscale);

}

void
FMODSoundSystem::set3DListenerAttributes(Int32 listener,
                                         const Vector3D* pos,
                                         const Vector3D* vel,
                                         const Vector3D* forward,
                                         const Vector3D* up) {
  result = fmodSoundSystem->
  set3DListenerAttributes(listener,
                          reinterpret_cast<const FMOD_VECTOR*>(pos),
                          reinterpret_cast<const FMOD_VECTOR*>(vel),
                          reinterpret_cast<const FMOD_VECTOR*>(forward),
                          reinterpret_cast<const FMOD_VECTOR*>(up));
}

void
FMODSoundSystem::getMasterChannelGroup(DrChannelGroup** channelGroup) {
  result = fmodSoundSystem->getMasterChannelGroup(
  reinterpret_cast<FMOD::ChannelGroup**>(channelGroup));
}

void
FMODSoundSystem::createDSPByType(DR_DSP_TYPE::E bspType,
                                 DrDSP **dsp) {
  result = fmodSoundSystem->createDSPByType(
  static_cast<FMOD_DSP_TYPE>(bspType),
  reinterpret_cast<FMOD::DSP **>(dsp));
}

}