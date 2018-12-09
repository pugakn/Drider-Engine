#include "dr_fmod_sound_api.h"

#include "dr_fmod_channel.h"
#include "dr_fmod_sound.h"
#include "dr_fmod_soundSystem.h"
#include "dr_fmod_reverb3d.h"
#include "dr_fmod_dsp.h"
#include "dr_fmod_channelGroup.h"

#include "dr_time.h"

namespace driderSDK {

void
FMODSoundAPI::init() {

  system = new FMODSoundSystem;
  system->init(32, DR_INITFLAGS::kDrInitFlags_NORMAL, 0);
  system->set3DSettings(1.0f, 100.0f, 1.0f); 

  masterGroup = new FMODChannelGroup;
  system->getMasterChannelGroup(reinterpret_cast<DrChannelGroup**>(masterGroup->getObjectReference()));
  dspLowPass = new FMODDSP;
  system->createDSPByType(DR_DSP_TYPE::FMOD_DSP_TYPE_FLANGE,
                          reinterpret_cast<DrDSP**>(dspLowPass->getObjectReference()));
  masterGroup->addDSP(0,
                      reinterpret_cast<DrDSP*>(dspLowPass->getReference()));
  dspLowPass->setBypass(true);

  channel1 = new FMODChannel;
  /*pos = Vector3D(-100.f, 0.f, 0.f);
  Vector3D vel(0.f,0.f,0.f);
  Vector3D fwd(0,0,1);
  Vector3D up(0,1,0);
  system->set3DListenerAttributes(4, &pos, &vel, &up, &up);
  channel1->set3DAttributes(&pos, &vel);*/
                            
  channel2 = new FMODChannel;
  channel3 = new FMODChannel;
  channel4 = new FMODChannel; 
}

void
FMODSoundAPI::update() {
  
  system->update();
}

void
FMODSoundAPI::destroy() {

}

void
FMODSoundAPI::selectChannel(Int32 indexChannel) {
  
}
  

}