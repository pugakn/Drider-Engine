#include <dr_aabb.h>
#include <dr_matrixnxm.h>
#include "dr_intersections.h"
#include "dr_sphere.h"
#include "dr_plane.h"
#include "dr_frustrum.h"
#include "dr_ray.h"

namespace driderSDK {

AABB::AABB() {

}

AABB::AABB(float s, const Vector3D& C) 
  : m_fSize(s),
    m_center(C) {

}

AABB::~AABB() {

}

bool
AABB::intersect(AABB& aabb) {
  return aabbAabbIntersect(m_center, m_fSize, aabb.getCenter(), aabb.getSize());
}

bool
AABB::intersect(Sphere& sphere) {
  return aabbSphereIntersect(m_center, m_fSize, sphere.getCenter(), sphere.getRadio());
}

bool
AABB::intersect(Plane& plane) {
  Vector3D planePoint = plane.getNormal() * plane.getGap();
  return aabbPlaneIntersect(m_center, m_fSize, plane.getNormal(), planePoint);
}

bool
AABB::intersect(Frustrum& frustrum) {
  return aabbFrustrumIntersect(m_center, m_fSize, frustrum.getVP());
}

bool
AABB::intersect(Ray& ray) {
  return aabbRayIntersect(m_center, m_fSize, ray.getOrigin(), ray.getDirection());
}

bool
AABB::intersect(Vector3D& point) {
  return pointInAabb(m_center, m_fSize, point);
}

} 