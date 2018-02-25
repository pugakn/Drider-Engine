#include <dr_quaternion.h>
#include "dr_camera.h"
#include "dr_time.h"
#include <iostream>

namespace driderSDK {

Camera::Camera() {}

Camera::Camera(const TString& name,
							 const Viewport& viewport) 
  : GameObject(name), 
    m_up(0.0f, 1.0f, 0.0f),
    m_viewport(viewport) {
}

Camera::~Camera() {}

void Camera::updateImpl() {
  GameObject::updateImpl();

  auto& tr = getParent()->transform;
  auto pos = getWorldTransform().getPosition();
  auto target = Vector4D(m_target, 1) * tr.getMatrix();
	m_view.LookAt(Vector3D(pos), Vector3D(target), m_up);
	m_vp = m_view * m_projection;
}

void 
Camera::move(float forward, float strafe, float upVelocity, bool lockY) {
	
  Vector3D dir = transform.getDirection();
  Vector3D right = dir.cross(m_up);

  if (lockY) {
    dir.y = 0;
    dir.normalize();
    right.y = 0; 
    right.normalize();
  }

  transform.move(dir * forward);
  
	if (strafe != 0.f) {
		transform.move(right * strafe);
	}

  transform.move(upVelocity, AXIS::kY);
}

void 
Camera::move(const Vector3D& direction) {
  transform.move(direction);
}

void 
Camera::pan(float forward, float strafe, float upVelocity, bool lockY) {
	
  Vector3D dir = transform.getDirection();
  Vector3D right = dir.cross(m_up);

  if (lockY) {
    dir.y = 0;
    dir.normalize();
    right.y = 0;
    right.normalize();
  }
  dir *= forward;
  dir += right * strafe + m_up * upVelocity;
  
	m_target += dir;

  move(dir);
}

void 
Camera::pan(const Vector3D & direction) {
  move(direction);
  m_target += direction;
}

void
Camera::createProyection(float fov,
												 float nearPlane,
												 float farPlane) {
	DR_ASSERT(m_viewport.height != 0.0f);
	m_projection.ProjectionFov(fov * Math::DEGREE_TO_RADIAN,
														static_cast<float>(m_viewport.width) / 
                                               m_viewport.height, 
														nearPlane,
														farPlane);
	//m_projection.Orthogonal(m_viewport.width, m_viewport.height, nearPlane, farPlane);
}

void 
Camera::setUp(const Vector3D& up) {
  m_up = up;
}

void 
Camera::setTarget(const Vector3D& target) {
	m_target = target;
}

void
Camera::setViewport(const Viewport& viewport) {
	m_viewport = viewport;
}

void 
Camera::rotate(const Quaternion& rotation) {
	m_target = transform.getPosition() + 
             rotation.rotation(transform.getDirection());
}

void 
Camera::rotate(float yawDegree, float pitchDegree) {
  transform.rotate({yawDegree * Math::DEGREE_TO_RADIAN,
                   pitchDegree * Math::DEGREE_TO_RADIAN, 0});
  m_target = transform.getPosition() + transform.getDirection() * 10.f;
}

void 
Camera::orbit(float pitch, float yaw) {
	//float distance =  Vector3D(m_pos - m_target).length(); 
	////placeholder xd
	//m_pos.x = distance * -sinf(pitch*(Math::PI / 180)) * cosf((yaw)*(Math::PI / 180));
	//m_pos.y = distance * -sinf((yaw)*(Math::PI / 180));
	//m_pos.z = -distance * cosf((pitch)*(Math::PI / 180)) * cosf((yaw)*(Math::PI / 180));

	//m_look = m_target - m_pos;
	//m_look.normalize();
}

const Matrix4x4& 
Camera::getVP() const {
  return m_vp;
}

const Matrix4x4& 
Camera::getView() const {
  return m_view;
}

const Matrix4x4&
Camera::getProjection() {
  return m_projection;
}

}