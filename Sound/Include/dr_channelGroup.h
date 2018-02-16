#pragma once
#include "dr_sound_prerequisites.h"

namespace driderSDK {

class DrDSP;

/**
* Channel group class 
*/
class DR_SOUND_EXPORT DrChannelGroup {
  public:
    /**
    * Default constructor
    */
    DrChannelGroup() {}

    /**
    * Default constructor
    */
    virtual ~DrChannelGroup() {}
   
    /**
    * Return void poiter to api object
    */
    virtual void*
    getReference() = 0;

    /**
    *
    */
    virtual void **
    getObjectReference() = 0;

    /**
    *
    */
    virtual void
    addDSP(Int32 index,
           DrDSP * dsp) = 0;
};
}