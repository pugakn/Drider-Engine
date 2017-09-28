#include <dr_ray.h>
#include <dr_intersections.h>
#include <dr_plane.h>
namespace driderSDK {
bool Ray::intersects(const Plane& plane) const
{
	return rayPlaneIntersection(m_origin, m_direction, plane, plane * plane.m_d);
}
bool Ray::intersects(const Plane & plane, float * t) const
{
	return rayPlaneIntersection(m_origin, m_direction, plane, plane * plane.m_d, t);
}
}