#pragma once

#include <array>
#include "dr_math_prerequisites.h"
#include "dr_vector3d.h"

namespace driderSDK {

class Matrix4x4;
class Plane;
class Sphere;
class Capsule;
class Frustrum;
class Ray;

namespace AABB_POINT {

enum E 
{
  kXMinYMinZMin,
  kXMaxYMinZMin,
  kXMinYMinZMax,
  kXMaxYMinZMax,
  kXMinYMaxZMin,
  kXMaxYMaxZMin,
  kXMinYMaxZMax,
  kXMaxYMaxZMax
};

}

/**
*  Axis aligned bounding box class with center, width, height and depth
*
* Sample usage:
*	AABB myAabb(5.0, 2.0, 2.0, driderSDK::Vector3D(0,0,0));
*/
class DR_MATH_EXPORT AABB
{
 public:
  
  /**
	* TEST::defaultConstructor
  * Default constructor
  *
  */
  AABB();

  /**
	* TEST::constructFromValues
  * Initialize the constructor with the given values
  *
  * @param width
  *  Width of the AABB
	*
	* @param height
	*  Height of the AABB
	*
	* @param depth
	*  Depth of the AABB
  *
  * @param C
  *  Center of the box given by a vector 3D
  */
  AABB(float _width, float _height, float _depth, const Vector3D& C);

  /**
	* TEST::moveConstructor
  * Move constructor.
  *
  */
  AABB(AABB&& A) = default;

  /**
	* TEST::constructFromAABB
  * Copy constructor.
  *
  */
  AABB(const AABB& A);

  /**
	* TEST::destructor
  * Default destructor
  *
  */
  ~AABB();

	/**
	* TEST::getMaxPoint
	* Gets the max point of the AABB.
	*
	* @return
	*  AABB's max point
	*
	*/
	const Vector3D
	getMaxPoint() const;

	/**
	* TEST::getMinPoint
	* Gets the min point of the AABB.
	*
	* @return
	*  AABB's min point
	*
	*/
	const Vector3D
	getMinPoint() const;

  /**
  * TEST::operatorEqual
  *
  * Set the  values of this AABB
  * with the values of the other AABB.
  *
  * @param A
  *  The AABB with the values to use.
  *
  * @return
  *   A reference to this AABB.
  */
  AABB&
  operator=(const AABB& A);

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
  intersect(AABB& aabb) const;

  /**
	* TEST::intersectSphere
  * Checks for an intersection with a sphere
  *
  * @param sphere
  *  The sphere to check
  *
  * @return
  *  True if it intersects
  */
  bool
  intersect(Sphere& sphere) const;

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
  intersect(Plane& plane) const;

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
  intersect(Frustrum& frustrum) const;

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
  intersect(Ray& ray) const;

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
  intersect(Vector3D& point) const;
    
  std::array<Vector3D, 8>
  getBounds() const;

  void
  recalculate(const Matrix4x4& transform);

  
	float width, height, depth;

	Vector3D center;
};

}