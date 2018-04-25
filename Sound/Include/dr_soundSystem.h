#pragma once
#include "dr_sound_prerequisites.h"
#include <memory>
#include "dr_sound_defines.h"
#include <dr_vector3d.h>

#include <dr_export_script.h>
#include <..\..\Script\Include\dr_script_engine.h>

namespace driderSDK {

class DrSound;
class DrChannelGroup;
class DrChannel;
class DrCreateSoundExInfo;
class DrDSP;

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
  * Gets the API's SoundSystem
  */
  virtual SoundSystem*
  get() = 0;

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

  /**
  *
  */
  virtual void
  playSound(DrSound* sound,
            DrChannelGroup* channelGroup,
            bool paused,
            DrChannel **channel) = 0;

  /**
  *
  */
  virtual void
  set3DSettings(float dopplerscale,
                float distancefactor,
                float rolloffscale) = 0;

  /**
  * This updates the position, velocity and orientation of the specified 3D sound listener.
  */
  virtual void
  set3DListenerAttributes(Int32 listener,
                          const Vector3D* pos,
                          const Vector3D* vel,
                          const Vector3D* forward,
                          const Vector3D* up) = 0;

  /**
  *
  */
  virtual void
  getMasterChannelGroup(DrChannelGroup** channelGroup) = 0;

  /**
  * 
  */
  virtual void
  createDSPByType(DR_DSP_TYPE::E bspType,
                  DrDSP **dsp) = 0;
 

};

}