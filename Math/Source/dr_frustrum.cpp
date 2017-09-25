#include <dr_frustrum.h>

namespace driderSDK {
Frustrum::Frustrum(const Matrix4x4& VP)
{
	setVP(VP);
}

void
driderSDK::Frustrum::setVP(const Matrix4x4 & VP)
{
	// Left clipping plane   
	m_planes[0].m_normal.x = VP.v0[3] + VP.v0[0];
	m_planes[0].m_normal.y = VP.v1[3] + VP.v1[0];
	m_planes[0].m_normal.z = VP.v2[3] + VP.v2[0];
	m_planes[0].m_d = VP.v3[3] + VP.v3[0];
	// Right clipping plane    
	m_planes[1].m_normal.x = VP.v0[3] - VP.v0[0];
	m_planes[1].m_normal.y = VP.v1[3] - VP.v1[0];
	m_planes[1].m_normal.z = VP.v2[3] - VP.v2[0];
	m_planes[1].m_d = VP.v3[3] - VP.v3[0];
	// Top clipping plane    
	m_planes[2].m_normal.x = VP.v0[3] - VP.v0[1];
	m_planes[2].m_normal.y = VP.v1[3] - VP.v1[1];
	m_planes[2].m_normal.z = VP.v2[3] - VP.v2[1];
	m_planes[2].m_d = VP.v3[3] - VP.v3[1];
	// Bottom clipping plane    
	m_planes[3].m_normal.x = VP.v0[3] + VP.v0[1];
	m_planes[3].m_normal.y = VP.v1[3] + VP.v1[1];
	m_planes[3].m_normal.z = VP.v2[3] + VP.v2[1];
	m_planes[3].m_d = VP.v3[3] + VP.v3[1];
	// Near clipping plane    
	m_planes[4].m_normal.x = VP.v0[2];
	m_planes[4].m_normal.y = VP.v1[2];
	m_planes[4].m_normal.z = VP.v2[2];
	m_planes[4].m_d = VP.v3[2];
	// Far clipping plane    
	m_planes[5].m_normal.x = VP.v0[3] - VP.v0[2];
	m_planes[5].m_normal.y = VP.v1[3] - VP.v1[2];
	m_planes[5].m_normal.z = VP.v2[3] - VP.v2[2];
	m_planes[5].m_d = VP.v3[3] - VP.v3[2];
	// Normalize the planes   
}
}
