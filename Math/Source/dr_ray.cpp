#include "dr_ray.h"
#include "dr_intersections.h"
#include "dr_plane.h"
#include "dr_sphere.h"
#include "dr_capsule.h"
#include "dr_aabb.h"

namespace driderSDK {

bool
Ray::intersects(const Plane& plane) const {
	return Intersect::rayPlane(origin, direction, plane, plane * plane.d);
}

bool
Ray::intersects(const Plane& plane, float* t) const {
	return Intersect::rayPlane(origin, direction, plane, plane * plane.d, t);
}

bool
Ray::intersects(const Sphere& sphere) const
{
  return Intersect::sphereRay(sphere.center, sphere.radius, origin, direction);
}

bool 
Ray::intersects(const Sphere& sphere, Vector3D* pointIntersect) const
{
  return Intersect::sphereRay(sphere.center,
                              sphere.radius,
                              origin,
                              direction,
                              pointIntersect);
}

bool 
Ray::intersects(const Capsule& capsule) const
{
  float point;
  return Intersect::rayCapsule(capsule.pointA,
                               capsule.pointB,
                               origin,
                               direction,
                               capsule.radius,
                               point);
}

bool
Ray::intersects(const AABB& aabb, Vector3D* point) {
  return Intersect::rayAABB(aabb.getMaxPoint(),
                            aabb.getMinPoint(),
                            origin,
                            direction,
                            point);
}

}