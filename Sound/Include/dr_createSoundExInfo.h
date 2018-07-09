#pragma once
#include "dr_sound_prerequisites.h"

namespace driderSDK {

/**
* Extra user data info
*/
class DR_SOUND_EXPORT DrCreateSoundExInfo {
  public:
    /**
    * Defult constructor
    */
    DrCreateSoundExInfo(){}

    /**
    * Default destructor
    */
    virtual ~DrCreateSoundExInfo() {}

    /**
    * Gets void pointer to the api object
    */
    virtual void*
    getReference() = 0;
};

}