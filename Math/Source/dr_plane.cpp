#include "dr_plane.h"
#include "dr_intersections.h"
#include "dr_ray.h"
#include "dr_aabb.h"
#include "dr_sphere.h"
#include "dr_capsule.h"
#include "dr_frustrum.h"

namespace driderSDK {

Plane::Plane() {}

Plane::Plane(const Vector3D& _normal, float _d)
  : Vector3D(_normal),
	  d(_d) {}

Plane::Plane(const Vector3D& _normal, const Vector3D& _point) {
  compute(_normal, _point);
}

Plane::Plane(const Vector3D& point0, 
			       const Vector3D& point1, 
			       const Vector3D& point2) {
  compute(point0, point1, point2);
}

Plane::Plane(const Plane& other)
  : Vector3D(other),
	  d(other.d) {}

void
Plane::compute(const Vector3D & point0,
               const Vector3D & point1, 
               const Vector3D & point2) {
  Vector3D normal = (point1 - point0).cross(point2 - point0);
  normal.normalize();
  compute(normal, point0);
}

void Plane::compute(const Vector3D& _normal, const Vector3D& point) {
  Vector3D::operator=(_normal);
  d = dot(point);
}

float
Plane::distanceToPoint(const Vector3D& point) {
  
  /******Formula*******
   	 N = Plane Normal 
   	 P = Point		  
   	 d = Plane gap

   	 |dot(N, P) + d|  
   	 ---------------  
      sqrt(dot(N,N))  
  *********************/

  //Warning: Assumes the normal is normalized
  return Math::abs(dot(point) + d);
}

Math::PLANE_INTERSECT 
Plane::intersects(const Vector3D& point) {
  //Ax+By+Cz = D
  //Ax + By + Cz - D = 0 If this is true the point is in the plane

  float signedDistance = (dot(point) + d) / length();
  
  if (signedDistance > Math::EPSILON) {
    return Math::kFront;
  }
  else if (signedDistance < -Math::EPSILON) {
    return Math::kBehind;
  }
  else {
    return Math::kIn;
  }
}

bool Plane::intersects(const Plane & other)
{
  return planePlaneIntersect(*this, d, other, other.d);
}

bool Plane::intersects(const Sphere & sphere)
{
  return spherePlaneIntersect(*this, d, sphere.center, sphere.radius);
}

bool Plane::intersects(const AABB & aabb)
{
  return aabbPlaneIntersect(aabb.center, aabb.width, aabb.height, *this, d);
}

bool Plane::intersects(const Capsule & capsule)
{
  return capsulePlaneIntersect(capsule.pointA,
                               capsule.pointB, 
                               capsule.radius,
                               *this,
                               d);
}

bool Plane::intersects(const Frustrum & frustrum)
{
  return frustrumPlaneIntersect(frustrum.planes, *this, d);
}

bool Plane::intersects(const Ray & ray) {
  return rayPlaneIntersect(ray.origin, ray.direction, *this, d);
}

void
Plane::normalize() {
  float invLength = 1.f / length();
  Vector3D::operator*=(invLength);
  d *= invLength;
}

Plane&
Plane::operator=(const Plane& other) {
  Vector3D::operator=(other);
  d = other.d;

  return *this;
}

bool
Plane::operator==(const Plane& rhs) {
  return Vector3D::operator==(rhs) && d == rhs.d;
}

bool
Plane::operator!=(const Plane& rhs) {
  return !(*this == rhs);
}

}