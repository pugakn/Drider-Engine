#pragma once
#include <dr_prerequisites.h>
#include "dr_vector3d.h"

namespace driderSDK {

class Plane;
class Sphere;
class Capsule;
class Frustrum;

/**
*  Ray class with origin and direction
*
* Sample usage:
*	Ray(Vector3D(0,0,0), Vector3D(1,0.5,0.9));
*/
class DR_API_EXPORT Ray
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
  Ray(Vector3D _origin, Vector3D _direction)
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
	* Check if the ray intersects with a sphere
	*
	*	@param sphere
	*	 The sphere to check the intersecton.
	*
	* @return
	*	  True if the ray intersects with the sphere
	*
	
	/*bool
	intersects(const Sphere& sphere) const;

	/**
	* Check if the ray intersects with a capsule
	*
	*	@param capsule
	*	 The capsule to check the intersecton.
	*
	* @return
	*	  True if the ray intersects with the capsule
	*
	*/
	/*bool
	intersects(const Capsule& capsule) const;*/

	/**
	* Check if the ray intersects with a frustrum
	*
	*	@param frustrum
	*	 The frustrum to check the intersecton.
	*
	* @return
	*	  True if the ray intersects with the frustrum
	*
	*/
	/*bool
	intersects(const Frustrum& frustrum) const;*/

	Vector3D origin;
	Vector3D direction;

};


}