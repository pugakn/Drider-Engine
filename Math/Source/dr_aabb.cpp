#include "dr_aabb.h"
#include "dr_matrix4x4.h"
#include "dr_vector4d.h"
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

AABB&
AABB::operator=(const AABB & A)
{
  width = A.width;
  height = A.height;
  depth = A.depth;
  center = A.center;
  return *this;
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
  return Intersect::aabbSphere(getMinPoint(), 
															 getMaxPoint(), 
                               sphere.center, 
                               sphere.radius);
}

bool
AABB::intersect(Plane& plane) {
  return Intersect::aabbPlane(center, 
															width,
															height,
	                            static_cast<Vector3D&>(plane), 
	                            plane.d);
}

bool
AABB::intersect(Frustrum& frustrum) {
  return Intersect::aabbFrustrum(getMaxPoint(),
                                 getMinPoint(),
																 frustrum.planes);
}

bool
AABB::intersect(Ray& ray) {
	return Intersect::aabbRay(getMaxPoint(), 
														getMinPoint(), 
														ray.origin, 
														ray.direction);
}

bool
AABB::intersect(Vector3D& point) {
  return Intersect::aabbPoint(getMaxPoint(), getMinPoint(), point);
}

std::array<Vector3D, 8>
AABB::getBounds() const {
  std::array<Vector3D, 8> points;

  Vector3D min = getMinPoint();

  points[AABB_POINT::kXMinYMinZMin] = min;
  points[AABB_POINT::kXMaxYMinZMin] = min + Vector3D(width,0,0);
  points[AABB_POINT::kXMinYMinZMax] = min + Vector3D(0,0,depth);
  points[AABB_POINT::kXMaxYMinZMax] = min + Vector3D(width,0,depth);
  points[AABB_POINT::kXMinYMaxZMin] = min + Vector3D(0,height,0);
  points[AABB_POINT::kXMaxYMaxZMin] = min + Vector3D(width,height,0);
  points[AABB_POINT::kXMinYMaxZMax] = min + Vector3D(0,height,depth);
  points[AABB_POINT::kXMaxYMaxZMax] = min + Vector3D(width, height, depth);

  return points;
}

void
AABB::recalculate(const Matrix4x4& transform) {
  //Left, Down, Back, Right, Up, Front
  //0 (-x,-y,-z)  L-D-B
  //1 (x, -y, -z) R-D-B
  //2 (-x, -y, z) L-D-F
  //3 (x, -y, z)  R-D-F
  //4 (-x, y, -z) L-U-B
  //5 (x, y, -z)  R-U-B
  //6 (-x, y, z)  L-U-F
  //7 (x, y, z)   R-U-F
  
  auto points = getBounds();

  Vector3D min{Math::MAX_FLOAT, Math::MAX_FLOAT, Math::MAX_FLOAT};
  Vector3D max{Math::MIN_FLOAT, Math::MIN_FLOAT, Math::MIN_FLOAT};

  for(auto& point : points) {

    Vector4D temp(point, 1);

    temp = temp * transform;

    for (Int32 i = 0; i < 3; ++i) {
      min[i] = Math::min(min[i], temp[i]);
      max[i] = Math::max(max[i], temp[i]);
    }  
  }

  auto size = max - min; 

  width = size.x;
  height = size.y;
  depth = size.z;
  center = (max + min) * 0.5f;
}

} 