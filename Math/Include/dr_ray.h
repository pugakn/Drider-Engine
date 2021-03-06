#pragma once

#include "dr_math_prerequisites.h"
#include "dr_vector3d.h"

namespace driderSDK {

class Plane;
class Sphere;
class Capsule;
class Frustrum;
class AABB;
class Line;

/**
*  Ray class with origin and direction
*
* Sample usage:
*	Ray(Vector3D(0,0,0), Vector3D(1,0.5,0.9));
*/
class DR_MATH_EXPORT Ray
{
 public:
  /**
  * TEST::defaultConstructor
  * Default constructor.
  *
  */
  Ray()
  {}

  /**
  * TEST::origingDirectionCons
  * Constructor using origin and direction
  *
  * @param _origin
  *	 The origin of the ray.
  *
  * @param _direction
  *  The direction of the ray
  *
  */
  Ray(const Vector3D& _origin, const Vector3D& _direction)
    : origin(_origin),
	    direction(_direction)
  {}

  /**
  * TEST::defaultDestructor
  * Destructor
  *
  */
  ~Ray()
  {}

	/**
  * TEST::intersectPlane
	* Check if the ray intersects with a plane
	*
	*	@param plane
	*	 The plane to check the intersecton.
	*
	* @return
	*	  True if the ray intersects with the plane
	*
	*/
	bool
	intersects(const Plane& plane) const;

	/**
  * TEST::intersectPlaneT
	* Check if the ray intersects with a plane
	*
	*	@param plane
	*	 The plane to check the intersecton.
	*
	* @out param t
	*	 The parameter of the point intersection.
	*
	* @return
	*	  True if the ray intersects with the plane
	*
	*/
	bool
	intersects(const Plane& plane, float* t) const;

	/**
  * TEST::intersectSphere
  *
	* Check if the ray intersects with a sphere
	*
	*	@param sphere
	*	 The sphere to check the intersecton.
	*
	* @return
	*	  True if the ray intersects with the sphere
	*
	*/
	bool
	intersects(const Sphere& sphere) const;

  /**
  * TEST::intersectSphere
  * Check if the ray intersects with a sphere
  *
  *	@param sphere
  *	 The sphere to check the intersecton.
  *
  * @return
  *	  True if the ray intersects with the sphere
  *
  */
  bool
  intersects(const Sphere& sphere, Vector3D* pointIntersect) const;

	/*
	* Check if the ray intersects with a capsule
	*
	*	@param capsule
	*	 The capsule to check the intersecton.
	*
	* @return
	*	  True if the ray intersects with the capsule
	*
	*/
  /*
	bool
	intersects(const Capsule& capsule) const;*/

  /**
  * TEST::intersectAABB
  * Check if the ray intersects with a aabb
  *
  *	@param aabb
  *	 The aabb to check the intersecton.
  *
  * @return
  *	  True if the ray intersects with the frustrum
  *
  */
  bool
  intersects(const AABB& aabb, Vector3D* point);

	/**
  * TEST::frustrumRay
	* Check if the ray intersects with a frustrum
	*
	*	@param frustrum
	*	 The frustrum to check the intersecton.
	*
	* @return
	*	  True if the ray intersects with the frustrum
	*
	*/
	bool
	intersects(const Frustrum& frustrum) const;

  /**
  * TEST::intersectRay
  * Check if the ray intersects with a ray
  *
  *	@param ray
  *	 The ray to check the intersecton.
  *
  * @return
  *	  True if the ray intersects with the ray
  *
  * @Point
  *	  Intersection point
  *
  */
  bool
  intersects(const Ray& ray, Vector3D* point) const;

  /*
  * TEST::intersectLine
  * Check if the ray intersects with a line
  *
  *	@param line
  *	 The line to check the intersecton.
  *
  * @return
  *	  True if the ray intersects with the line
  *
  * @Point
  *	  Intersection point
  *
  */
  /*bool
  intersects(const Line& line, Vector3D* point) const;*/

	Vector3D origin;
	Vector3D direction;

};


}