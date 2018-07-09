#pragma once
#include "dr_sound_prerequisites.h"
#include "dr_sound_defines.h"
#include <dr_vector3d.h>

namespace driderSDK {

class DR_SOUND_EXPORT DrReverb3D {

  public:
    /**
    *  Default constructor
    */
    DrReverb3D() {}

    /**
    * Default constructor
    */
    virtual ~DrReverb3D() {}

    /**
    * Get reference to object
    */
    virtual void*
    getRefence() = 0;
    
    /**
    * Get reference to direction of object
    */
    virtual void**
    getObjectReference() = 0;
    
    /**
    * Set reverb properties
    */
    virtual void
    setProperties(const DrReverb3DProperties *properties) = 0;

    /**
    * Set reverb3d atributes
    *
    * @param position
    * The position of the reverb
    *
    * @param minDistance
    * Minmum distance to hear the reverb
    *
    * @param maxDistance
    * Maximum distance to hear the reverb
    */
    virtual void
    set3DAttributes(const Vector3D *position,
                    float minDistance,
                    float maxDistance) = 0;
};

}