#pragma once
#include "dr_sound_prerequisites.h"

namespace driderSDK{

class SoundSystem;
class DrChannel;

struct DR_SOUND_EXPORT SoundExtraInfo {

  public:
    SoundExtraInfo(SoundSystem *soundSystem, DrChannel *channel) :
                   m_soundSystem(soundSystem), m_channel(channel){}

    ~SoundExtraInfo() {}

    SoundSystem *m_soundSystem;
    DrChannel *m_channel;

};  

}