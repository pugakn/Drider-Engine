#pragma once
#include "dr_sound_prerequisites.h"
#include "dr_reverb3d.h"

#include <FMOD\fmod.hpp>

namespace driderSDK {

class DR_SOUND_EXPORT FMODReverb3D : public DrReverb3D {

  public:
    FMODReverb3D() {}
    virtual ~FMODReverb3D() {}

    void *
    getRefence() override;

    void **
    getObjectReference() override;

    void
    setProperties(const DrReverb3DProperties *properties) override;

    void
    set3DAttributes(const Vector3D *position,
                    float minDistance,
                    float maxDistance) override;

  private:
    FMOD::Reverb3D* reverb3D;
    FMOD_RESULT result;

};

}