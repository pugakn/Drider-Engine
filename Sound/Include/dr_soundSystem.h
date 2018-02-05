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
  //SoundSystem() {}
  /**
  * Default destructor
  */
  virtual ~SoundSystem() {}  

  /**
  * Initialize the sound system
  *
  * @param maxChannels
  * Max number of channels to use for sound systems
  *
  * @param initFlags
  * Initilize flag
  *
  * @param extraDriverData
  * Extra user info
  */
  virtual DR_SOUND_RESULT::E
  init(Int32 maxChannels,
       DR_INITFLAGS::E initFlags,
       void *extraDriverData) = 0;
  
  /**
  * Update the sound system
  */
  virtual void
  update() = 0;

  /**
  * Gets void pointer to sound system
  */
  virtual void*
  getReference() = 0;

  /**
  * Gets void pointer to directions of sound system
  */
  virtual void**
  getObjectReference() = 0;

  /**
  * Creates a sound
  * @param name
  * Name of the resource
  * 
  * @param mode
  * Mode of the sound
  *
  * @param createInfo
  * Extra information of creation
  *
  * @param sound
  * Pointer to resource api sound
  */
  virtual void 
  createSound(TString name,
              DR_SOUND_MODE::E mode,
              DrCreateSoundExInfo *createInfo,
              DrSound * sound) = 0;

};

}