#include <dr_quaternion.h>
#include "dr_camera.h"
#include "dr_time.h"
#include <iostream>
#include <dr_gameComponent.h>

namespace driderSDK {

CONSTRUCT_DESTRUCT_DEF(Camera)

Camera::Camera() 
  : Camera({0, 100, -100}, {0, 100, 1}) {
  
}

Camera::Camera(const Vector3D& _position, 
               const Vector3D& _target)
  : m_up(0.0f, 1.0f, 0.0f),
    m_farPlane(0),
    m_nearPlane(0),
    m_fov(0),
    m_viewport{0,0,100,100}, 
    m_position(_position),
    m_target(_target),
    m_outdateView(true) {
}

Camera::~Camera() {}

void
Camera::move(float forward, float strafe, float upVelocity, bool lockY) {
    
}

void
Camera::move(const Vector3D& direction) {
  m_position += direction;
}

void
Camera::pan(float forward, float strafe, float upVelocity, bool lockY) {

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
  m_farPlane = farPlane;
  m_nearPlane = nearPlane;
  m_fov = fov;
	
  m_projection.ProjectionFov(m_fov * Math::DEGREE_TO_RADIAN,
                              static_cast<float>(m_viewport.width) / 
                                                 m_viewport.height,
                             m_nearPlane, 
                             m_farPlane);

  m_vp = m_view * m_projection;
}

void
Camera::setUp(const Vector3D& up) {
  m_up = up;

  invalidateView();
}

void
Camera::setTarget(const Vector3D& target) {
	m_target = target;

  invalidateView();
}

void
Camera::setViewport(const Viewport& viewport) {
	
  m_viewport = viewport;

  createProyection(m_fov, m_nearPlane, m_farPlane);
}

void
Camera::rotate(const Quaternion& rotation) {
	
}

void
Camera::rotate(float yawDegree, float pitchDegree) {
  
}

void
Camera::orbit(float pitch, float yaw) {
	
}

const Matrix4x4&
Camera::getVP() const {

  if (m_outdateView) {
    getView();
  }

  return m_vp;
}

const Matrix4x4&
Camera::getProjection() const {
  return m_projection;
}

const Matrix4x4&
Camera::getView() const {

  if (m_outdateView) {
    m_view.LookAt(m_position, m_target, m_up);
    m_outdateView = false;
    m_vp = m_view * m_projection;
  }
  
  return m_view;
}

float 
Camera::getFarPlane() const {
  return m_farPlane;
}

float 
Camera::getNearPlane() const {
  return m_nearPlane;
}

float 
Camera::getFOV() const {
  return m_fov;
}

void 
Camera::invalidateView() {
  m_outdateView = true;
}


}
