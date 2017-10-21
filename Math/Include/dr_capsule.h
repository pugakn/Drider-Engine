#pragma once
#include <dr_prerequisites.h>
#include "dr_vector3d.h"

namespace driderSDK {

class Sphere;
class Plane;
class AABB;
class Frustrum;

class DR_API_EXPORT Capsule
{
 public:

  Capsule() {}

  ~Capsule() {}

  Vector3D pointA;
  Vector3D pointB;
  float radius;
  
  bool
  intersects(const Plane& plane);
  bool
  intersects(const Sphere& sphere);
  bool
  intersects(const AABB& aabb);
  bool
  intersects(const Capsule& capsule);
  bool
  intersects(const Frustrum& frustrum);
 protected:
 private:
};

}