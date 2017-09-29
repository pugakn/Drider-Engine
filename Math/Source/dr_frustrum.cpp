#include <dr_frustrum.h>

namespace driderSDK {
Frustrum::Frustrum(const Matrix4x4& ViewProjection)
{
	createFromVP(ViewProjection);
}

void
driderSDK::Frustrum::createFromVP(const Matrix4x4& ViewProjection)
{
	// Left clipping plane   
	m_planes[0].x = ViewProjection.vector0[3] + ViewProjection.vector0[0];
	m_planes[0].y = ViewProjection.vector1[3] + ViewProjection.vector1[0];
	m_planes[0].z = ViewProjection.vector2[3] + ViewProjection.vector2[0];
	m_planes[0].d = ViewProjection.vector3[3] + ViewProjection.vector3[0];
	// Right clipping plane    
	m_planes[1].x = ViewProjection.vector0[3] - ViewProjection.vector0[0];
	m_planes[1].y = ViewProjection.vector1[3] - ViewProjection.vector1[0];
	m_planes[1].z = ViewProjection.vector2[3] - ViewProjection.vector2[0];
	m_planes[1].d = ViewProjection.vector3[3] - ViewProjection.vector3[0];
	// Top clipping plane    
	m_planes[2].x = ViewProjection.vector0[3] - ViewProjection.vector0[1];
	m_planes[2].y = ViewProjection.vector1[3] - ViewProjection.vector1[1];
	m_planes[2].z = ViewProjection.vector2[3] - ViewProjection.vector2[1];
	m_planes[2].d = ViewProjection.vector3[3] - ViewProjection.vector3[1];
	// Bottom clipping plane    
	m_planes[3].x = ViewProjection.vector0[3] + ViewProjection.vector0[1];
	m_planes[3].y = ViewProjection.vector1[3] + ViewProjection.vector1[1];
	m_planes[3].z = ViewProjection.vector2[3] + ViewProjection.vector2[1];
	m_planes[3].d = ViewProjection.vector3[3] + ViewProjection.vector3[1];
	// Near clipping plane    
	m_planes[4].x = ViewProjection.vector0[2];
	m_planes[4].y = ViewProjection.vector1[2];
	m_planes[4].z = ViewProjection.vector2[2];
	m_planes[4].d = ViewProjection.vector3[2];
	// Far clipping plane    
	m_planes[5].x = ViewProjection.vector0[3] - ViewProjection.vector0[2];
	m_planes[5].y = ViewProjection.vector1[3] - ViewProjection.vector1[2];
	m_planes[5].z = ViewProjection.vector2[3] - ViewProjection.vector2[2];
	m_planes[5].d = ViewProjection.vector3[3] - ViewProjection.vector3[2];
	// Normalize the planes   
	m_planes[0].normalize();
	m_planes[1].normalize();
	m_planes[2].normalize();
	m_planes[3].normalize();
	m_planes[4].normalize();
	m_planes[5].normalize();
}
}
