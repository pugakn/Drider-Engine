#include <dr_ray.h>
#include <dr_intersections.h>
#include <dr_plane.h>
namespace driderSDK {
bool Ray::intersects(const Plane& plane) const
{
	Vector3D n =  plane.getNormal();
	return rayPlaneIntersection(m_origin, m_direction, plane.getNormal(), plane.getNormal() * plane.getGap());
}
bool Ray::intersects(const Plane & plane, float * t) const
{
	return rayPlaneIntersection(m_origin, m_direction, plane.getNormal(), plane.getNormal() * plane.getGap(), t);
}
}