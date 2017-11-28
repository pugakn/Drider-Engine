#include "dr_aabb.h"
#include "dr_matrixnxm.h"
#include "dr_intersections.h"
#include "dr_sphere.h"
#include "dr_plane.h"
#include "dr_frustrum.h"
#include "dr_ray.h"

namespace driderSDK {

AABB::AABB() {}

AABB::AABB(float _width, 
           float _height, 
					 float _depth,
           const Vector3D& C) {
	width = _width; 
	height = _height; 
	depth = _depth;
	center = C;
}

AABB::AABB(const AABB& A) {
	width = A.width;  
	height = A.height;
	depth = A.depth;
	center = A.center;
}

AABB::~AABB() {}

const Vector3D
AABB::getMaxPoint() const {
	return Vector3D(center.x + (width * 0.5f),
									center.y + (height * 0.5f),
									center.z + (depth * 0.5f));
}

const Vector3D
AABB::getMinPoint() const {
	return Vector3D(center.x - (width * 0.5f),
									center.y - (height * 0.5f),
									center.z - (depth * 0.5f));
}

bool
AABB::intersect(AABB& aabb) {
  return Intersect::aabbAabb(center, 
														 width,
														 height,
														 depth,
	                           aabb.center,
														 aabb.width,
														 aabb.height,
														 aabb.depth);
}

bool
AABB::intersect(Sphere& sphere) {
  return Intersect::aabbSphere(center, 
															 width, 
															 height, 
															 depth,
                               sphere.center, 
                               sphere.radius);
}

bool
AABB::intersect(Plane& plane) {
  return Intersect::aabbPlane(center, 
															width,
															height,
															depth,
	                            static_cast<Vector3D&>(plane), 
	                            plane.d);
}

bool
AABB::intersect(Frustrum& frustrum) {
  return Intersect::aabbFrustrum(center, 
																 width,
																 height,
																 depth,
																 frustrum.planes);
}

bool
AABB::intersect(Ray& ray) {
	return Intersect::aabbRay(center, 
														getMaxPoint(), 
														getMinPoint(), 
														ray.origin, 
														ray.direction);
}

bool
AABB::intersect(Vector3D& point) {
  return Intersect::aabbPoint(center, getMaxPoint(), getMinPoint(), point);
}

} 