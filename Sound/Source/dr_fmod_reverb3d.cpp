#include "dr_fmod_reverb3d.h"

namespace driderSDK {

void *
FMODReverb3D::getRefence() {
  return reverb3D;
}

void **
FMODReverb3D::getObjectReference() {
  return reinterpret_cast<void**>(&reverb3D);
}

void
FMODReverb3D::setProperties(const DrReverb3DProperties *properties) {
  result = reverb3D->setProperties(
  reinterpret_cast<const FMOD_REVERB_PROPERTIES*>
  (properties));
}

void
FMODReverb3D::set3DAttributes(const Vector3D *position,
                              float minDistance,
                              float maxDistance) {
  result = reverb3D->set3DAttributes(reinterpret_cast<
                                     const FMOD_VECTOR*>
                                     (position),
                                     minDistance,
                                     maxDistance);
}


}