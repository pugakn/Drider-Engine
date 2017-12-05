#pragma once

#include "dr_math_prerequisites.h"
#include "dr_vector3d.h"

namespace driderSDK {

namespace PLANE_INTERSECT {
enum E;
}

class Ray;
class Sphere;
class Capsule;
class AABB;
class Frustrum;
class Line;

class DR_MATH_EXPORT Plane : public Vector3D
{
 public:
  /**
  * TEST::constructorDefault
  *	Default constructor.
  */
  Plane();

  /**
  * TEST::destructor
  * Destructor.
  */
  ~Plane();

  /**
  * TEST::constructorNormalGap
  * Constructor which takes a normal and a gap .
  */
  Plane(const Vector3D& _normal, float _d);

  /**
  * TEST::constructorNormalPoint
  *	Constructor using a normal and a point to calculate the gap.
  */
  Plane(const Vector3D& _normal, const Vector3D& point);

  /**
  * TEST::constructorThreePoints
  * Constructor using 3 points in the plane which are used to 
  *	calculate the normal of the plane and the gap.
  */
  Plane(const Vector3D& point0, 
		    const Vector3D& point1, 
		    const Vector3D& point2);

  /**
  * TEST::constructorCopy
  *	Copy constructor
  */
  Plane(const Plane& other);

  /*
  * TEST:distanceToPoint
  * Computes the distance from the plane to a point
  * 
  * @param point
  *  The point from which distance is measured.
  * 
  * @return distance to the point.
  *
  */
  float 
  distanceToPoint(const Vector3D& point) const;
   
  /**
  * TEST::intersectsPoint
  * Computes the relationship between a point and the plane.
  *
  *	@return
  *	  The relationship of the point with the plane.
  */
  PLANE_INTERSECT::E
  intersects(const Vector3D& point);

  /**
  * TEST::intersectsPlane
  */
  bool 
  intersects(const Plane& other);

  /**
  * TEST::intersectsSphere
  */
  bool 
  intersects(const Sphere& sphere);

  /**
  * TEST::intersectsAABB
  */
  bool 
  intersects(const AABB& aabb);

  /**
  * TEST::intersectsCapsule
  */
  bool 
  intersects(const Capsule& capsule);

  /**
  * TEST::intersectsFrustrum
  */
  bool 
  intersects(const Frustrum& frustrum);

  /**
  * TEST::intersectsRay
  */
  bool 
  intersects(const Ray& ray);

  /**
  * TEST::normalize
  * Normalizes the plane. This modifies the normal and the gap. 
  */
  void 
  normalize();

  /**
  * TEST::operatorCopyAssignment
  */
  Plane& 
  operator=(const Plane& other);

  /**
  * TEST::operatorEquality
  */
  bool
  operator==(const Plane& rhs);

  /**
  * TEST::operatorInequality
  */
  bool 
  operator!=(const Plane& rhs);

  //gap
  float d;
 protected:
 private:
};

}