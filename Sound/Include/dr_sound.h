#pragma once
#include "dr_sound_prerequisites.h"
#include <dr_soundExtraInfo.h>

namespace driderSDK {

class SoundSystem;
class DrChannel;

/**
* Sound object to control the sound api object of sound api
*/
class DR_SOUND_EXPORT DrSound {
  public:
    //DrSound() {}

    /**
    * Default destructor
    */
    virtual ~DrSound() {}
    
    /**
    * Initialize the sound
    *
    * @param system
    * Pointer to sound system
    *
    * @param channel
    * Pointer to sound channel
    */
    virtual void
    init(SoundSystem *system,
         DrChannel **channel) = 0;

    virtual void
    setSystem (SoundSystem *system) = 0;

    virtual void
    setChannel(DrChannel** channel) = 0;

    /**
    * Gets void pointer to api sound
    */
    virtual void*
    getReference() = 0;

    /**
    * Gets pointer to direction of api sound
    */
    virtual void**
    getObjectReference() = 0;

    /**
    * Return the sounds api
    */
    virtual DrSound*
    get() = 0;


    /**
    * Plays the sound
    */
    virtual void
    play() = 0;

    /**
    * Set the mode of the sound
    *
    * @param mode
    * Mode sound
    */
    virtual void
    setMode(DR_SOUND_MODE::E mode) = 0;

    /**
    *
    */
    virtual void
    set3DMinMaxDistance(float min,
                        float max) = 0;

};

}