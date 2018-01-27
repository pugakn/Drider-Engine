#pragma once
#include "dr_sound_prerequisites.h"
#include <memory>
#include "dr_sound_defines.h"

namespace driderSDK {

class DrSound;
class DrChannelGroup;
class DrChannel;
class DrCreateSoundExInfo;

class DR_SOUND_EXPORT SoundSystem {
public:
  SoundSystem() {}
  virtual ~SoundSystem() {}

  virtual void 
  init(Int32 maxChannels,
       DR_INITFLAGS::E initFlags,
       void *extraDriverData) = 0;

  /*virtual void*
  getReference() = 0;

  virtual DR_SOUND_RESULT::E createSound(const char *name,
                                         DR_SOUND_MODE::E mode,
                                         DrCreateSoundExInfo *createInfo,
                                         DrSound ** sound) = 0;

  virtual DR_SOUND_RESULT::E playSound(DrSound *sound,
                                       DrChannelGroup *channelgroup,
                                       bool paused,
                                       DrChannel *channel) = 0;*/
};

}