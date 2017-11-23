#pragma once

#include "dr_math_prerequisites.h"
#include "dr_vector3d.h"

namespace driderSDK {
class Plane;
class AABB;
class Capsule;
class Frustrum;

class DR_MATH_EXPORT Sphere
{
 public:
  /**
  * TEST::sphereConstructor
  *	Default constructor.
  */
  Sphere() {}

  /**
  * TEST::sphereConstructor
  *	Constructor with center and radius.
  */
  Sphere(Vector3D _center,
         float _radius);

  /**
  * TEST::sphereDestructor
  *	Destructor.
  */

  ~Sphere() {}
 
  /**
  * TEST::intersectsPoint
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
  * TEST::intersectsPlane
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
  * TEST::intersectsSphere
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
  * TEST::intersectsAabb
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
  * TEST::intersectsCapsule
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
  * TEST::intersectsFrustrum
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