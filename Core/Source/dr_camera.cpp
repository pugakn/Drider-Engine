#include <dr_quaternion.h>
#include "dr_camera.h"


namespace driderSDK {

Camera::Camera() {}

Camera::Camera(const TString& name,
							 const Vector3D& pos,
							 const Vector3D& target,
							 const Viewport& viewport,
							 float fov,
							 float nearPlane,
							 float farPlane) {
	m_name = name;
	m_pos = pos;
	m_target = target;
	m_look = Vector3D(m_target - m_pos).normalize();
	m_viewport = viewport;
	m_up = Vector3D(0.f, 1.f, 0.f);

	createProyection(fov, nearPlane, farPlane);
}

Camera::~Camera() {}

void 
Camera::update(float delta) {
	m_view.LookAt(m_pos, m_pos + m_look, m_up);
	m_vp = m_view * m_projection;
}

void 
Camera::move(float forward, float strafe) {
	m_pos += m_look*forward;
	if (strafe != 0.f) {
		m_pos += Vector3D(m_look.cross(m_up)*strafe).normalize();
	}
	m_look = m_target - m_pos;
	m_look.normalize();
}

void 
Camera::pan(float forward, float strafe) {
	m_pos += m_look*forward;
	m_target += m_look*forward;

	if (strafe != 0.f) {
		m_pos += Vector3D(m_look.cross(m_up)*strafe).normalize();
		m_target += Vector3D(m_look.cross(m_up)*strafe).normalize();
	}

}

void
Camera::createProyection(float fov,
												 float nearPlane,
												 float farPlane) {
	DR_ASSERT(m_viewport.height != 0.0f);
	m_projection.ProjectionFov(fov * Math::DEGREE_TO_RADIAN,
														m_viewport.width / m_viewport.height, 
														nearPlane,
														farPlane);
	//m_projection.Orthogonal(m_viewport.width, m_viewport.height, nearPlane, farPlane);
}

void 
Camera::setTarget(const Vector3D& target) {
	m_target = target;
}

void
Camera::setViewport(const Viewport& viewport) {
	m_viewport = viewport;
}


const TString&
Camera::getName() const {
	return m_name;
}

void 
Camera::rotate(const Quaternion& rotation) {
	m_look = rotation.rotation(m_look);
}

void 
Camera::orbit(float pitch, float yaw) {
	float distance =  Vector3D(m_pos - m_target).length(); 
	//placeholder xd
	m_pos.x = distance * -sinf(pitch*(Math::PI / 180)) * cosf((yaw)*(Math::PI / 180));
	m_pos.y = distance * -sinf((yaw)*(Math::PI / 180));
	m_pos.z = -distance * cosf((pitch)*(Math::PI / 180)) * cosf((yaw)*(Math::PI / 180));

	m_look = m_target - m_pos;
	m_look.normalize();
}

const Matrix4x4& 
Camera::getVP() const {
  return m_vp;
}

}