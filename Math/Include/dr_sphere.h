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
  /**
  *	Default constructor.
  */
  Sphere() {}

  /**
  *	Constructor with center and radius.
  */
  Sphere(Vector3D _center,
         float _radius);
  ~Sphere() {}
 
  /**
  * Checks if a point is on a sphare.
  *
  * @param point
  *   The point to check.
  *
  * @return
  *   Return true if the point is in the sphere, otherwise return false.
  */
  bool 
  intersects(const Vector3D& point);

  /**
  * Checks if a plane intersets with a sphere.
  *
  * @param plane
  *   The plane to check.
  *
  * @return
  *   Return true if the plane intersects with the sphere, otherwise return false.
  */
  bool 
  intersects(const Plane& plane);

  /**
  * Checks if a sphere intersets with other sphere.
  *
  * @param sphere
  *   The sphere to check.
  *
  * @return
  *   Return true if the sphere intersects with the other sphere, otherwise return false.
  */
  bool 
  intersects(const Sphere& sphere);

  /**
  * Checks if a aabb intersets with a sphere.
  *
  * @param aabb
  *   The aabb to check.
  *
  * @return
  *   Return true if the sphere intersects with the aabb, otherwise return false.
  */
  bool 
  intersects(const AABB& aabb);

  /**
  * Checks if a capsule intersets with a sphere.
  *
  * @param capsule
  *   The capsule to check.
  *
  * @return
  *   Return true if the sphere intersects with the capsule, otherwise return false.
  */
  bool 
  intersects(const Capsule& capsule);

  /**
  * Checks if a frustrum intersets with a sphere.
  *
  * @param frustrum
  *   The frustrum to check.
  *
  * @return
  *   Return true if the sphere intersects with the frustrum, otherwise return false.
  */
  bool 
  intersects(const Frustrum& frustrum);
  
  float radius;
  Vector3D center;
};

}