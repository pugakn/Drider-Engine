#pragma once

#include <dr_prerequisites.h>
#include "dr_vector3d.h"

namespace driderSDK {

class Plane;
class Sphere;
class Capsule;
class Frustrum;
class Ray;

/**
*  Axis aligned bounding box class with center and length
*
* Sample usage:
*	AABB myAabb(5.0, 2.0, driderSDK::Vector3D(0,0,0));
*/
class DR_API_EXPORT AABB
{
 public:
  
  /**
  * Default constructor
  *
  */
  AABB();

  /**
  * Initialize the constructor with the given values
  *
  * @param s
  *  Size of the AABB
  *
  * @param C
  *  Center of the box given by a vector 3D
  */
  AABB(float width, float height, const Vector3D& C);

  /**
  * Move constructor.
  *
  */
  AABB(AABB&& A) = default;

  /**
  * Copy constructor.
  *
  */
  AABB(const AABB& A);

  /**
  * Default destructor
  *
  */
  ~AABB();

  /**
	* TEST::intersectAABB
  * Checks for an intersection with another AABB
  *
  * @param aabb
  *  The other AABB to check
  *
  * @return
  *  True if it intersects
  */
  bool
  intersect(AABB& aabb);

  /**
  * Checks for an intersection with a sphere
  *
  * @param sphere
  *  The sphere to check
  *
  * @return
  *  True if it intersects
  */
  bool
  intersect(Sphere& sphere);

  /**
	* TEST::intersectPlane
  * Checks for an intersection with a plane
  *
  * @param plane
  *  The plane to check
  *
  * @return
  *  True if it intersects
  */
  bool
  intersect(Plane& plane);

  /**
	* TEST::intersectFrustrum
  * Checks for an intersection with a frustrum
  *
  * @param frustrum
  *  The frustrum to check
  *
  * @return
  *  True if it intersects
  */
  bool
  intersect(Frustrum& frustrum);

  /**
	* TEST::intersectRay
  * Checks for an intersection with a ray
  *
  * @param ray
  *  The ray to check
  *
  * @return
  *  True if the ray intersects with the AABB
  */
  bool
  intersect(Ray& ray);

  /**
	* TEST::intersectPoint
  * Checks if the point is in the AABB
  *
  * @param point
  *  Vector3D that represents the point
  *
  * @return
  *  True if the point is inside
  */
  bool
  intersect(Vector3D& point);
  
  float width, height;
  Vector3D center;
};

}