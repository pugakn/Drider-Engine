#pragma once

#include <dr_prerequisites.h>
#include "dr_vector3d.h"

namespace driderSDK {

class DR_API_EXPORT Plane
{
 public:
  /**
  *	Default constructor
  */
  Plane();

  /**
  * Constructor which takes a normal and a gap 
  */
  Plane(const Vector3D& _normal, Float32 _d);

  /**
  *	Constructor using a normal and a point to calculate the gap
  */
  Plane(const Vector3D& _normal, const Vector3D& point);

  /**
  * Constructor using 3 points in the plane which are used to 
  *	calculate the normal of the plane and the gap
  */
  Plane(const Vector3D& point0, 
		const Vector3D& point1, 
		const Vector3D& point2);

  /**
  *	Copy constructor
  */
  Plane(const Plane& other);

  Float32 distanceToPoint(const Vector3D& point);

  Vector3D getNormal() const;
  Float32 getGap() const;

  Plane& operator=(const Plane& other);

  bool operator==(const Plane& rhs);
  bool operator!=(const Plane& rhs);

 protected:
 private:
	friend class Frustrum;
  Vector3D m_normal;
  Float32 m_d;
};

}