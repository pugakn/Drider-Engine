#include <dr_quaternion.h>
#include "dr_camera.h"
#include "dr_viewport.h"

namespace driderSDK {

Camera::Camera() {}

Camera::Camera(const Vector3D& pos,
							 const Vector3D& target,
							 float fov,
							 float nearPlane,
							 float farPlane, 
               Viewport* _pViewport) : m_viewport(_pViewport) {
	m_pos = pos;
	m_target = target;
	m_up = Vector3D(0.f, 1.f, 0.f);

  //m_projection.Orthogonal(m_viewport->width, m_viewport->height, nearPlane, farPlane);
	m_projection.ProjectionFov(fov * Math::DEGREE_TO_RADIAN,
														 m_viewport->width / m_viewport->height,
														 nearPlane,
														 farPlane);
	
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
Camera::setTarget(const Vector3D& target) {
	m_target = target;
}

void
Camera::setViewport(const Viewport* _viewport) {

}

void 
Camera::rotate(const Quaternion& rotation) {
	m_target = rotation.rotation(m_target);
}

void 
Camera::orbit() {

}

const Matrix4x4& 
Camera::getVP() const {
  return m_vp;
}

}