#include <dr_sphere.h>

#include "dr_plane.h"
#include "dr_aabb.h"
#include "dr_capsule.h"
#include "dr_frustrum.h"
#include "dr_intersections.h"

namespace driderSDK {

Sphere::Sphere(Vector3D _center,
               float _radius) : 
               center(_center),
               radius(_radius) {
}

bool
Sphere::intersects(const Vector3D& point) {
  return Intersect::sphereContainsPoint(point, center, radius);
}

bool
Sphere::intersects(const Plane& plane) {
  return Intersect::spherePlane(plane, plane.d, center, radius);
}
bool
Sphere::intersects(const Sphere& sphere) {
  return Intersect::sphereSphere(center, radius, sphere.center, sphere.radius);
}
bool
Sphere::intersects(const AABB& aabb) {
  return Intersect::aabbSphere(aabb.getMinPoint(), 
															 aabb.getMaxPoint(), 
															 center, 
															 radius);
}
bool
Sphere::intersects(const Capsule& capsule) {
  return Intersect::sphereCapsule(center, 
																	radius, 
																	capsule.pointA, 
																	capsule.pointB, 
																	capsule.radius);
}
bool
Sphere::intersects(const Frustrum& frustrum) {
  return Intersect::frustrumSphere(frustrum.planes, center, radius); // frustrumSphere(); Error frustrum no implemented
}

}