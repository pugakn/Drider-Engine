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
   /**
   *TEST::capsuleConstructor
   *Default constructor.
   */
  Capsule() {}
  
  /**
  * TEST::capsuleConstructor
  *	Default with two points an radius.
  */
  Capsule(const Vector3D& _pointA, 
          const Vector3D& _pointB, 
          float _radius);
  
  /**
  * TEST::capsuleDestructor
  */
  ~Capsule() {}
  
  /**
  * TEST::intersectPlane 
  * Checks if the capsule intersects with a plane.
  *
  * @param plane
  *   The plane to check.
  *
  * @return
  *   Return true if the plane intertsects with sphare, 
  *   otherwise return false.
  */
  bool
  intersects(const Plane& plane);

  /**
  * TEST::intersectSphere
  * Checks if the capsule intersects with a plane.
  *
  * @param sphere
  *   The sphere to check.
  *
  * @return
  *   Return true if the sphere intertsects with other sphere, 
  *   otherwise return false.
  */
  bool
  intersects(const Sphere& sphere);

  /**
  * TEST::intersectAabb
  * Checks if the capsule intersects with a plane.
  *
  * @param aabb
  *   The aabb to check.
  *
  * @return
  *   Return true if the sphare intertsects with aabb,
  *   otherwise return false.
  */
  bool
  intersects(const AABB& aabb);

  /**
  * TEST::intersectCapsule
  * Checks if the capsule intersects with a other.
  *
  * @param capsule
  *   The capsule to check.
  *
  * @return
  *   Return true if the capsule intertsects with capsule,
  *   otherwise return false.
  */
  bool
  intersects(const Capsule& capsule);

  /**
  * TEST::intersectFrustrum
  * Checks if the capsule intersects with a frustrum.
  *
  * @param frustrum
  *   The frustrum to check.
  *
  * @return
  *   Return true if the capsule intertsects with frustrum,
  *   otherwise return false.
  */
  bool
  intersects(const Frustrum& frustrum);

  Vector3D pointA;
  Vector3D pointB;
  float radius;

 protected:
 private:
};

}