#include "dr_frustrum.h"
#include "dr_sphere.h"

namespace driderSDK {
Frustrum::Frustrum(const Matrix4x4& ViewProjection) {
	createFromVP(ViewProjection);
}

void
driderSDK::Frustrum::createFromVP(const Matrix4x4& ViewProjection) {
	// Left clipping plane   
	planes[0].x = ViewProjection.vector0[3] + ViewProjection.vector0[0];
	planes[0].y = ViewProjection.vector1[3] + ViewProjection.vector1[0];
	planes[0].z = ViewProjection.vector2[3] + ViewProjection.vector2[0];
	planes[0].d = ViewProjection.vector3[3] + ViewProjection.vector3[0];
	// Right clipping plane    
	planes[1].x = ViewProjection.vector0[3] - ViewProjection.vector0[0];
	planes[1].y = ViewProjection.vector1[3] - ViewProjection.vector1[0];
	planes[1].z = ViewProjection.vector2[3] - ViewProjection.vector2[0];
	planes[1].d = ViewProjection.vector3[3] - ViewProjection.vector3[0];
	// Top clipping plane    
	planes[2].x = ViewProjection.vector0[3] - ViewProjection.vector0[1];
	planes[2].y = ViewProjection.vector1[3] - ViewProjection.vector1[1];
	planes[2].z = ViewProjection.vector2[3] - ViewProjection.vector2[1];
	planes[2].d = ViewProjection.vector3[3] - ViewProjection.vector3[1];
	// Bottom clipping plane    
	planes[3].x = ViewProjection.vector0[3] + ViewProjection.vector0[1];
	planes[3].y = ViewProjection.vector1[3] + ViewProjection.vector1[1];
	planes[3].z = ViewProjection.vector2[3] + ViewProjection.vector2[1];
	planes[3].d = ViewProjection.vector3[3] + ViewProjection.vector3[1];
	// Near clipping plane    
	planes[4].x = ViewProjection.vector0[2];
	planes[4].y = ViewProjection.vector1[2];
	planes[4].z = ViewProjection.vector2[2];
	planes[4].d = ViewProjection.vector3[2];
	// Far clipping plane    
	planes[5].x = ViewProjection.vector0[3] - ViewProjection.vector0[2];
	planes[5].y = ViewProjection.vector1[3] - ViewProjection.vector1[2];
	planes[5].z = ViewProjection.vector2[3] - ViewProjection.vector2[2];
	planes[5].d = ViewProjection.vector3[3] - ViewProjection.vector3[2];
	// Normalize the planes   
	planes[0].normalize();
	planes[1].normalize();
	planes[2].normalize();
	planes[3].normalize();
	planes[4].normalize();
	planes[5].normalize();
}

bool
Frustrum::intersects(const Sphere& sphere) const {
  return Intersect::frustrumSphere(planes, sphere.center, sphere.radius);
}
}
