#include "dr_frustrum.h"
#include "dr_sphere.h"
#include "dr_aabb.h"
#include "dr_ray.h"
namespace driderSDK {
Frustrum::Frustrum(const Matrix4x4& view, const Matrix4x4& projection) {
	createFromVP(view, projection);
}

void
driderSDK::Frustrum::createFromVP(Matrix4x4 view, 
                                  Matrix4x4 projection) {

  float size = 1;

  std::array<Vector3D, 8> vertices = {
    Vector3D{-size, size, size}, Vector3D{size, size, size},
    Vector3D{-size, size, 0},    Vector3D{size, size, 0},
    Vector3D{-size, -size, size},Vector3D{size, -size, size},
    Vector3D{-size, -size, 0},   Vector3D{size, -size, 0}
  };

  view.inverse();
  projection.inverse();

  for (auto& vertex : vertices) {    
    Vector4D A = Vector4D(vertex,1) * projection;
    A = A * view;
    vertex = A;
    vertex /= A.w;
  }  
 
  /*Far Plane*/
  planes[0] = Plane(Vector3D(vertices[4]), 
                    Vector3D(vertices[5]),
                    Vector3D(vertices[0]));

  /*Near plane*/
  planes[5] = Plane(Vector3D(vertices[7]), 
                    Vector3D(vertices[6]),
                    Vector3D(vertices[3]));

  /*Left*/
  planes[1] = Plane(Vector3D(vertices[4]), 
                    Vector3D(vertices[0]),
                    Vector3D(vertices[6]));

  /*Right*/
  planes[2] = Plane(Vector3D(vertices[7]), 
                    Vector3D(vertices[3]),
                    Vector3D(vertices[5]));

  /*Top*/
  planes[3] = Plane(Vector3D(vertices[2]), 
                    Vector3D(vertices[0]),
                    Vector3D(vertices[3]));
  
  /*Down*/
  planes[4] = Plane(Vector3D(vertices[7]), 
                    Vector3D(vertices[5]),
                    Vector3D(vertices[6]));
}

bool
Frustrum::intersects(const Ray& bRay) const
{
  Intersect::rayFrustrum(bRay.origin, bRay.direction, planes);
  return false;
}

bool
Frustrum::intersects(const Sphere& sphere) const {
  return Intersect::frustrumSphere(planes, sphere.center, sphere.radius);
}


FRUSTRUM_INTERSECT::E
Frustrum::intersects(const AABB& aabb) const
{
  return Intersect::aabbFrustrum(aabb.getMaxPoint(), aabb.getMinPoint(), planes);
}
}
