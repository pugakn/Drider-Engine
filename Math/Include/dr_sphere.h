#pragma once

#include <dr_prerequisites.h>
#include "dr_vector3d.h"
#include "dr_intersections.h"

namespace driderSDK {
class Plane;
class AABB;
class Capsule;
class Frustrum;

class DR_API_EXPORT Sphere
{
 public:
  //Uncommented
  Sphere()
  {}
  float radius;
  Vector3D center;
  //void SetRadio();
  //void SetPosition();
  
  bool 
  intersects(const Vector3D& point);
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

};

}