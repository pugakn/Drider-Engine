#pragma once
#include "dr_sound_prerequisites.h"

namespace driderSDK {

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
};
}