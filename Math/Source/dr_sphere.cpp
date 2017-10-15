#include <dr_sphere.h>

#include "dr_plane.h"
#include "dr_aabb.h"
#include "dr_capsule.h"
#include "dr_frustrum.h"

namespace driderSDK {

bool
Sphere::intersects(const Vector3D& point) {
  return sphereContainsPoint(point, center, radius);
}

bool
Sphere::intersects(const Plane& plane) {
  return plane.d > 0; // Error, Intersects plane no implemented
}
bool
Sphere::intersects(const Sphere& sphere) {
  return sphereSphereIntersect(center, radius, sphere.center, sphere.radius);
}
bool
Sphere::intersects(const AABB& aabb) {
  return aabbSphereIntersect(aabb.center, aabb.width, aabb.height, center, radius);
}
bool
Sphere::intersects(const Capsule& capsule) {
  return sphereCapsuleIntersect(center, radius, capsule.pointA, capsule.pointB, capsule.radius);
}
bool
Sphere::intersects(const Frustrum& frustrum) {
  return frustrum.kBottom > 0; // frustrumSphereIntersect(); Error frustrum no implemented
}
}