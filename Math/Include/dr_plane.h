#pragma once

#include <dr_prerequisites.h>
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

class DR_API_EXPORT Plane : public Vector3D
{
 public:
  /**
  * TEST::sphereSphere
  *	Default constructor.
  */
  Plane();

  /**
  * TEST::sphereCapsule
  * Constructor which takes a normal and a gap .
  */
  Plane(const Vector3D& _normal, float _d);

  /**
  * TEST::capsuleCapsule
  *	Constructor using a normal and a point to calculate the gap.
  */
  Plane(const Vector3D& _normal, const Vector3D& point);

  /**
  * TEST::sphereRay
  * Constructor using 3 points in the plane which are used to 
  *	calculate the normal of the plane and the gap.
  */
  Plane(const Vector3D& point0, 
		    const Vector3D& point1, 
		    const Vector3D& point2);

  /**
  * TEST::rayPlane
  *	Copy constructor
  */
  Plane(const Plane& other);

  /**
  * TEST::rayCapsule
  * Computes the normal and gap of the plain from 3 points.
  */
  void 
  compute(const Vector3D& point0, 
		      const Vector3D& point1, 
		      const Vector3D& point2);
  /**
  * TEST::frustrumFrustrum
  * Destructor.
  */
  ~Plane();

  /**
  * TEST::rayPlane
  * Computes the normal and gap of the plain from a normal and 1 point.
  */
  void 
  compute(const Vector3D& _normal, const Vector3D& point);

  float 
  distanceToPoint(const Vector3D& point);
   
  /**
  * TEST::frustrumSphere
  * Computes the relationship between a point and the plane.
  *
  *	@return
  *	  The relationship of the point with the plane.
  */
  PLANE_INTERSECT::E
  intersects(const Vector3D& point);
  /**
  * TEST::aabbAabb 
  */
  bool 
  intersects(const Plane& other);
  /**
  * TEST::aabbSphere
  */
  bool 
  intersects(const Sphere& sphere);
  /**
  * TEST::aabbRay
  */
  bool 
  intersects(const AABB& aabb);
  /**
  * TEST::aabbPoint
  */
  bool 
  intersects(const Capsule& capsule);
  /**
  * TEST::spherePlane
  */
  bool 
  intersects(const Frustrum& frustrum);
  /**
  * TEST::aabbPlane
  */
  bool 
  intersects(const Ray& ray);

  /**
  * TEST::segmentPlane
  * Normalizes the plane. This modifies the normal and the gap. 
  */
  void 
  normalize();
  /**
  * TEST::capsulePlane
  */
  Plane& 
  operator=(const Plane& other);
  /**
  * TEST::frustrumPlane
  */
  bool
  operator==(const Plane& rhs);
  /**
  * TEST::planePlane
  */
  bool 
  operator!=(const Plane& rhs);
  //gap
  float d;

 protected:
 private:
};

}