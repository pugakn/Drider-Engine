#include "dr_fmod_sound_api.h"

#include "dr_fmod_channel.h"
#include "dr_fmod_sound.h"
#include "dr_fmod_soundSystem.h"
#include "dr_fmod_reverb3d.h"

namespace driderSDK {

void
FMODSoundAPI::init() {

  system = new FMODSoundSystem;
  system->init(32, DR_INITFLAGS::kDrInitFlags_NORMAL, 0);
  system->set3DSettings(1.0f, 100.0f, 1.0f);

  channel1 = new FMODChannel;
  Vector3D pos(-100.f, 0.f, 0.f);
  Vector3D vel(0.f,0.f,0.f);
  channel1->set3DAttributes(&pos, &vel);
                            
  channel2 = new FMODChannel;
  channel3 = new FMODChannel;
  channel4 = new FMODChannel;

  reverb3D1 = new FMODReverb3D;
 
}

void
FMODSoundAPI::update() {
  system->update();
}

void
FMODSoundAPI::destroy() {

}


}