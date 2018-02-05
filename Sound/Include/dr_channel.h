#pragma once
#include "dr_sound_prerequisites.h"
#include <dr_vector3d.h>

namespace driderSDK {

/**
* The channel class controls the characteristis of the channel.
*/
class DR_SOUND_EXPORT DrChannel {
  public:
    //DrChannel() {}
    
    /**
    * Default constructor
    */
    virtual ~DrChannel() {}

    /**
    * Get reference to the channel object
    */
    virtual void*
    getReference() = 0;

    /**
    * Get reference to the channel
    */
    virtual void**
    getObjectReference() = 0;


    /**
    * Pause and realease the channel
    *
    * @param paused
    * Bool to pause and release the channel
    */
    virtual void
    setPaused(bool paused) = 0;

    /**
    * Gets pause state of the channel in paused param
    *
    * @param paused
    * Return variable of pause state
    */
    virtual void
    getPaused(bool *paused) = 0;

    /**
    * Set volume of channel
    *
    * @param volume
    * Linear volume level, default = 1.0f
    */
    virtual void
    setVolume(float volume) = 0;


    /**
    * Get volume of channel
    *
    * @param volume
    * Address of a variable to receive the linear volume level
    */
    virtual void
    getVolume(float *volume) = 0;

    /**
    * Sets the wet level (or send level) of a particular reverb instance.
    */
    virtual void
    setReverbProperties (Int32 instance,
                         float wet) = 0;

    /**
    *
    */
    virtual void
    set3DAttributes(const Vector3D* pos,
                    const Vector3D* vel) = 0;

};

}