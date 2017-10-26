#include <dr_capsule.h>

#include "dr_intersections.h"
#include "dr_sphere.h"
#include "dr_plane.h"
#include "dr_aabb.h"
#include "dr_frustrum.h"

namespace driderSDK {

Capsule::Capsule(const Vector3D& _pointA,
        const Vector3D& _pointB,
        float _radius) :
        pointA(_pointA),
        pointB(_pointB),
        radius(_radius) {};

bool
Capsule::intersects(const Plane& plane) {
  return Intersect::capsulePlane(pointA, pointB, radius, 
                                 plane, plane.d);
}


bool
Capsule::intersects(const Sphere& sphere) {
  return Intersect::sphereCapsule(sphere.center, 
                                  sphere.radius, 
                                  pointA, 
                                  pointB, 
                                  radius);
}

bool
Capsule::intersects(const AABB& aabb) {
  return aabb.height > 0; //Error, AabbCapsule no implemented
}


bool
Capsule::intersects(const Capsule& capsule) {
  return Intersect::capsuleCapsule(pointA, 
                                   pointB, 
                                   radius, 
                                   capsule.pointA, 
                                   capsule.pointB,
                                   capsule.radius);
}

bool
Capsule::intersects(const Frustrum& frustrum) {
  return frustrum.kFar > 0; // Error, frustrum no implemented
}
}