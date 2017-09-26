#pragma once

#include <dr_prerequisites.h>
#include "dr_vector3d.h"

namespace driderSDK {

class Ray;
class Sphere;
class Capsule;
class AABB;
class Frustrum;

class DR_API_EXPORT Plane
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

  float distanceToPoint(const Vector3D& point);

  /**
  *	Computes the signed distance to a point.
  *
  *	@param point
  *	 The point used to calculate the distance.
  *
  *	@return 
  *	  The signed distance to the point, this is useful
  *	  to know if a point is behind or in front of the plane.
  */
  float signedDistanceToPoint(const Vector3D& point);
  
  /**
  * Computes the intersection with a point
  *
  *	@return
  *	  True if the point is in the plane, false otherwise
  */
  bool intersects(const Vector3D& point);
  bool intersects(const Plane& other);
  bool intersects(const Sphere& sphere);
  bool intersects(const AABB& aabb);
  bool intersects(const Capsule& capsule);
  bool intersects(const Frustrum& frustrum);

  /**
  * Gets the plane normalized
  * 
  * @return
  *	  The plane normalized.
  */
  Plane normalize() const;

  Vector3D getNormal() const;

  float getGap() const;

  Plane& operator=(const Plane& other);

  bool operator==(const Plane& rhs);

  bool operator!=(const Plane& rhs);

 protected:
 private:
  Vector3D m_normal;
  float m_d;
};

}