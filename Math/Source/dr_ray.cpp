#include "dr_ray.h"
#include "dr_intersections.h"
#include "dr_plane.h"
namespace driderSDK {

bool Ray::intersects(const Plane& plane) const {
	return Intersect::rayPlane(origin, direction, plane, plane * plane.d);
}

bool Ray::intersects(const Plane& plane, float* t) const {
	return Intersect::rayPlane(origin, direction, plane, plane * plane.d, t);
}

}