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
               const Vector3D& _target,
               const TString& _name)
  : m_up(0.0f, 1.0f, 0.0f),
    m_farPlane(0),
    m_nearPlane(0),
    m_fov(0),
    m_viewport{0,0,100,100}, 
    m_position(_position),
    m_target(_target),
    m_name(_name),
    m_outdateView(true) {
}

Camera::~Camera() {}

void 
Camera::setPosition(const Vector3D& position) {
  m_position = position;
  invalidateView();
}

void
Camera::move(float forward, float strafe, float upVelocity, bool lockY) {
  auto dir = (m_target - m_position).normalize();
  auto upDir = m_up.normalize();
  auto strafeDir = (upDir.cross(dir)).normalize();
  m_position += dir * forward;
  m_position += upDir * upVelocity;
  m_position += strafeDir * strafe;
  m_target += dir * forward;
  m_target += upDir * upVelocity;
  m_target += strafeDir * strafe;
  invalidateView();
}

void
Camera::move(const Vector3D& direction) {
  m_position += direction;
  invalidateView();
}

void
Camera::pan(float forward, float strafe, float upVelocity, bool lockY) {
  auto dir = (m_target - m_position).normalize();
  auto upDir = m_up.normalize();
  auto strafeDir = (upDir.cross(dir)).normalize();
  m_position += dir * forward;
  m_position += upDir * upVelocity;
  m_position += strafeDir * strafe;
  invalidateView();
}

void
Camera::pan(const Vector3D& direction) {
  move(direction);
  m_target += direction;
  invalidateView();
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
Camera::createProyection(float width,
                         float height,
                         float nearPlane,
                         float farPlane) {
  m_farPlane = farPlane;
  m_nearPlane = nearPlane;
  m_fov = 0;
	
  m_projection.Orthogonal(width, height, m_nearPlane, m_farPlane);

  m_vp = getView() * m_projection;
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
  invalidateView();
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

TString
Camera::getName() const {
  return m_name;
}

UInt32
Camera::getViewportWidth() const {
  return m_viewport.width;
}

UInt32
Camera::getViewportHeight() const {
  return m_viewport.height;
}

Vector3D
Camera::getPosition() const {
  return m_position;
}

Vector3D
Camera::getDirection() const {
  return (m_target - m_position).normalize();
}

Vector3D
Camera::getLocalRight() const {
  Vector3D result = m_up.cross(getDirection());
  return result.normalize();
}

Vector3D
Camera::getLocalUp() const {
  Vector3D result = getDirection().cross(getLocalRight());
  return result.normalize();
}

void 
Camera::invalidateView() {
  m_outdateView = true;
}


}
