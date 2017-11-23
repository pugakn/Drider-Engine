#include "dr_ray.h"
#include "dr_intersections.h"
#include "dr_plane.h"
#include "dr_sphere.h"

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

}