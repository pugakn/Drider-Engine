#include "dr_frustrum.h"
#include "dr_sphere.h"
#include "dr_aabb.h"
#include "dr_ray.h"
namespace driderSDK {
Frustrum::Frustrum(const Matrix4x4& ViewProjection) {
	createFromVP(ViewProjection);
}

void
driderSDK::Frustrum::createFromVP(const Matrix4x4& ViewProjection) {
	// Left clipping plane   
	planes[kLeft].x = ViewProjection.vector3[0] + ViewProjection.vector0[0];
	planes[kLeft].y = ViewProjection.vector3[1] + ViewProjection.vector0[1];
	planes[kLeft].z = ViewProjection.vector3[2] + ViewProjection.vector0[2];
	planes[kLeft].d = ViewProjection.vector3[3] + ViewProjection.vector0[3];
  planes[kLeft].normalize();

	// Right clipping plane    
  planes[kRight].x = ViewProjection.vector3[0] - ViewProjection.vector0[0];
  planes[kRight].y = ViewProjection.vector3[1] - ViewProjection.vector0[1];
  planes[kRight].z = ViewProjection.vector3[2] - ViewProjection.vector0[2];
  planes[kRight].d = ViewProjection.vector3[3] - ViewProjection.vector0[3];
  planes[kRight].normalize();

	// Top clipping plane    
	planes[kTop].x = ViewProjection.vector3[0] + ViewProjection.vector1[0];
	planes[kTop].y = ViewProjection.vector3[1] + ViewProjection.vector1[1];
	planes[kTop].z = ViewProjection.vector3[2] + ViewProjection.vector1[2];
	planes[kTop].d = ViewProjection.vector3[3] + ViewProjection.vector1[3];
  planes[kTop].normalize();

	// Bottom clipping plane    
  planes[kBottom].x = ViewProjection.vector3[0] - ViewProjection.vector1[0];
  planes[kBottom].y = ViewProjection.vector3[1] - ViewProjection.vector1[1];
  planes[kBottom].z = ViewProjection.vector3[2] - ViewProjection.vector1[2];
  planes[kBottom].d = ViewProjection.vector3[3] - ViewProjection.vector1[3];
  planes[kBottom].normalize();
  
  // Near clipping plane    
	planes[kNear].x = ViewProjection.vector2[0];
	planes[kNear].y = ViewProjection.vector2[1];
	planes[kNear].z = ViewProjection.vector2[2];
	planes[kNear].d = ViewProjection.vector2[3];
	planes[kNear].normalize();

	// Far clipping plane    
	planes[kFar].x = ViewProjection.vector3[0] - ViewProjection.vector2[0];
	planes[kFar].y = ViewProjection.vector3[1] - ViewProjection.vector2[1];
	planes[kFar].z = ViewProjection.vector3[2] - ViewProjection.vector2[2];
	planes[kFar].d = ViewProjection.vector3[3] - ViewProjection.vector2[3];
	planes[kFar].normalize();

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


bool
Frustrum::intersects(const AABB& aabb) const
{
  return Intersect::aabbFrustrum(aabb.getMaxPoint(), aabb.getMinPoint(), planes);
}
}
