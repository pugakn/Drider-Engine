#include "dr_aabb.h"
#include "dr_matrixnxm.h"
#include "dr_intersections.h"
#include "dr_sphere.h"
#include "dr_plane.h"
#include "dr_frustrum.h"
#include "dr_ray.h"

namespace driderSDK {

AABB::AABB() {}

AABB::AABB(float width, 
           float height, 
           const Vector3D& C) : width(width), height(height), center(C) {}

AABB::AABB(const AABB& A) {
  width = A.width;
  height = A.height;
  center = A.center;
}

AABB::~AABB() {}

bool
AABB::intersect(AABB& aabb) {
  return aabbAabbIntersect(center, 
                           width, 
                           height,
	                         aabb.center, 
                           aabb.height,
	                         aabb.width);
}

bool
AABB::intersect(Sphere& sphere) {
  return aabbSphereIntersect(center, 
                             width,
                             height, 
                             sphere.center, 
                             sphere.radius);
}

bool
AABB::intersect(Plane& plane) {
  return aabbPlaneIntersect(center, 
                            width,
                            height,
	                          static_cast<Vector3D&>(plane), 
	                          static_cast<Vector3D&>(plane) * plane.d);
}

bool
AABB::intersect(Frustrum& frustrum) {
  return aabbFrustrumIntersect(center, 
                               width,
                               height,
	                             frustrum.planes);
}

bool
AABB::intersect(Ray& ray) {
  return aabbRayIntersect(center, 
                          width,
                          height,
	                        ray.origin, 
	                        ray.direction);
}

bool
AABB::intersect(Vector3D& point) {
  return aabbPointIntersect(center, width, height, point);
}

} 