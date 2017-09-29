#pragma once
#include <dr_prerequisites.h>
#include "dr_vector3d.h"

namespace driderSDK {

class Plane;
class Sphere;
class Capsule;
class Frustrum;
class Ray;

class DR_API_EXPORT Capsule
{
 public:

  Capsule()
  {}

  Vector3D pointA;
  Vector3D pointB;
  float radio;
 protected:
 private:
};

}