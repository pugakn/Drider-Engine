#include <dr_aabb.h>
#include <dr_matrixnxm.h>

#include "dr_intersections.h"
#include "dr_sphere.h"
#include "dr_plane.h"
#include "dr_frustrum.h"
#include "dr_ray.h"

namespace driderSDK {

AABB::AABB() {}

AABB::AABB(float s, const Vector3D& C) : m_fSize(s), m_center(C) {}

AABB::AABB(const AABB& A) {
  m_fSize = A.m_fSize;
  m_center = A.m_center;
}

AABB::~AABB() {}

bool
AABB::intersect(AABB& aabb) {
  return aabbAabbIntersect(m_center, 
                           m_fSize, 
	                       aabb.m_center, 
	                       aabb.m_fSize);
}

bool
AABB::intersect(Sphere& sphere) {
  return aabbSphereIntersect(m_center, 
                             m_fSize, 
                             sphere.getCenter(), 
                             sphere.getRadio());
}

bool
AABB::intersect(Plane& plane) {
  return aabbPlaneIntersect(m_center, 
                            m_fSize, 
	                          static_cast<Vector3D&>(plane), 
	                          static_cast<Vector3D&>(plane) * plane.d);
}

bool
AABB::intersect(Frustrum& frustrum) {
  return aabbFrustrumIntersect(m_center, 
                               m_fSize, 
	                             frustrum.getVP());
}

bool
AABB::intersect(Ray& ray) {
  return aabbRayIntersect(m_center, 
                          m_fSize, 
	                        ray.origin, 
	                        ray.direction);
}

bool
AABB::intersect(Vector3D& point) {
  return pointInAabb(m_center, m_fSize, point);
}

} 