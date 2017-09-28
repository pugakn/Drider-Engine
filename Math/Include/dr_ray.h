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
  * Default constructor.
  *
  */
  Ray()
  {}

  /**
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
    : m_origin(_origin),
	    m_direction(_direction)
  {}

  /**
  * Destructor
  *
  */
  ~Ray()
  {}

  //Uncommented
  FORCEINLINE void
  setOrigin(const Vector3D& _origin) 
  {
    m_origin = _origin;
  }

  //Uncommented
  FORCEINLINE Vector3D
  getOrigin() const
  {
	  return m_origin;
  }

  //Uncommented
  FORCEINLINE void
  setDirection(const Vector3D& _direction)
  {
	  m_direction = _direction;
  }

  //Uncommented
  FORCEINLINE Vector3D
  getDirection() const
  {
	  return m_direction;
  }

	/**
	* Check if the ray intersects other ray
	*
	*	@param bRay
	*	 The other ray to check the intersecton.
	*
	* @return
	*	  True if the ray intersects with the other ray
	*
	*/
	bool
  intersects(const Ray& bRay) const;

	/**
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
	*/
	bool
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
	bool
	intersects(const Capsule& capsule) const;

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
	bool
	intersects(const Frustrum& frustrum) const;


 private:
	 Vector3D m_origin;
	 Vector3D m_direction;
};


}