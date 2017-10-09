#pragma once

#include <dr_prerequisites.h>
#include "dr_vector3d.h"
#include "dr_math.h"

namespace driderSDK {

class Ray;
class Sphere;
class Capsule;
class AABB;
class Frustrum;
class Line;

class DR_API_EXPORT Plane : public Vector3D
{
 public:
  /**
  *	Default constructor.
  */
  Plane();

  /**
  * Constructor which takes a normal and a gap .
  */
  Plane(const Vector3D& _normal, float _d);

  /**
  *	Constructor using a normal and a point to calculate the gap.
  */
  Plane(const Vector3D& _normal, const Vector3D& point);

  /**
  * Constructor using 3 points in the plane which are used to 
  *	calculate the normal of the plane and the gap.
  */
  Plane(const Vector3D& point0, 
		    const Vector3D& point1, 
		    const Vector3D& point2);

  /**
  *	Copy constructor
  */
  Plane(const Plane& other);

  /**
  * Computes the normal and gap of the plain from 3 points.
  */
  void 
  compute(const Vector3D& point0, 
		      const Vector3D& point1, 
		      const Vector3D& point2);

  /**
  * Computes the normal and gap of the plain from a normal and 1 point.
  */
  void 
  compute(const Vector3D& _normal, const Vector3D& point);

  float 
  distanceToPoint(const Vector3D& point);
   
  /**
  * Computes the relationship between a point and the plane.
  *
  *	@return
  *	  The relationship of the point with the plane.
  */
  Math::PLANE_INTERSECT 
  intersects(const Vector3D& point);

  bool 
  intersects(const Plane& other);

  bool 
  intersects(const Plane& other, Line& intersectionLine);

  bool 
  intersects(const Sphere& sphere);

  bool 
  intersects(const AABB& aabb);

  bool 
  intersects(const Capsule& capsule);

  bool 
  intersects(const Frustrum& frustrum);

  bool 
  intersects(const Ray& ray);

  /**
	* Checks if a ray intersects with the plane.
	*
	*	@param ray
	*	 The ray to check the intersecton.
	*
	* @out intersection
	*	 The intersection point parameter.
	*
	* @return
	*	  True if the ray intersects with the plane, false otherwise.
	*/
  bool 
  intersects(const Ray& ray, float& intersection);

  /**
  * Normalizes the plane. This modifies the normal and the gap. 
  */
  void 
  normalize();
  
  Plane& 
  operator=(const Plane& other);

  bool
  operator==(const Plane& rhs);

  bool 
  operator!=(const Plane& rhs);

  //gap
  float d;

 protected:
 private:
};

}