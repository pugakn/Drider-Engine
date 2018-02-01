#pragma once
#include "dr_sound_prerequisites.h"

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

};

}