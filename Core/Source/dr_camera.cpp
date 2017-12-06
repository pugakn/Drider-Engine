#include <dr_quaternion.h>
#include "dr_camera.h"


namespace driderSDK {

Camera::Camera() {}

Camera::Camera(const std::string& name,
							 const Vector3D& pos,
							 const Vector3D& target,
							 const Viewport& viewport,
							 float fov,
							 float nearPlane,
							 float farPlane) {
	m_name = name;
	m_pos = pos;
	m_target = target;
	m_viewport = viewport;
	m_up = Vector3D(0.f, 1.f, 0.f);

	createProyection(fov, nearPlane, farPlane);
}

Camera::~Camera() {}

void 
Camera::update(float delta) {
	m_view.LookAt(m_pos, m_target, m_up);
	m_vp = m_view * m_projection;
}

void 
Camera::move(float forward, float strafe) {
	m_pos += m_target*forward;
	m_pos += Vector3D(m_target.cross(m_up)*strafe).normalize();
}

void 
Camera::pan() {

}

void
Camera::createProyection(float fov,
												 float nearPlane,
												 float farPlane) {
	DR_ASSERT(m_viewport.height != 0.0f);
	m_projection = m_projection.ProjectionFov(fov * Math::DEGREE_TO_RADIAN,
																						m_viewport.width / m_viewport.height, 
																						nearPlane,
																						farPlane);
}

void 
Camera::setTarget(const Vector3D& target) {
	m_target = target;
}

void
Camera::setViewport(const Viewport& viewport) {
	m_viewport = viewport;
}

std::string&
Camera::getName() {
	return m_name;
}

void 
Camera::rotate(const Quaternion& rotation) {
	m_target = rotation.rotation(m_target);
}

void 
Camera::orbit() {

}

}