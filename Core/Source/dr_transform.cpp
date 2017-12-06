#include "dr_transform.h"
#include <dr_degree.h>
#include <dr_radian.h>

namespace driderSDK {

Transform::Transform() 
  : m_outdatedTransform(true),
    m_position(0,0,0),
    m_scale(1,1,1),
    m_rotation(0,0,0) {
}

const Matrix4x4& Transform::getTransformMatrix() {  

  if (m_outdatedTransform) {
    update();
  }

  return m_transform;
}

const Vector3D& Transform::getPosition() const {
  return m_position;
}

const Vector3D& Transform::getRotation() const {
  return m_rotation;
}

const Vector3D& Transform::getScale() const {
  return m_scale;
}

void Transform::setPosition(float pos, AXIS::E axis) {
  m_position[axis] = pos;
  invalidate();
}

void Transform::setPosition(const Vector3D & position) {
  m_position = position;
  invalidate();
}

void Transform::move(float dist, AXIS::E axis) {
  m_position[axis] += dist;
  invalidate();
}

void Transform::move(const Vector3D& distance) {
  m_position += distance;
  invalidate();
}

void Transform::setRotation(Radian angle, AXIS::E axis) {
  m_rotation[axis] = angle;
  invalidate();
}

void Transform::setRotation(Degree angle, AXIS::E axis) {
  m_rotation[axis] = angle.toRadian();
  invalidate();
}

void Transform::setRotation(const Vector3D& orientation) {
  m_rotation = orientation;
  invalidate();
}

void Transform::rotate(Radian angle, AXIS::E axis) {
  m_rotation[axis] += angle;
    invalidate();
}

void Transform::rotate(Degree angle, AXIS::E axis) {
  m_rotation[axis] += angle.toRadian();
  invalidate();
}

void Transform::rotate(const Vector3D & orientation) {
  m_rotation += orientation;
  invalidate();
}

void Transform::setScale(float scale, AXIS::E axis) {
  m_scale[axis] = scale;
  invalidate();
}

void Transform::setScale(const Vector3D& scale) {
  m_scale = scale;
  invalidate();
}

void Transform::scale(float scale, AXIS::E axis) {
  m_scale[axis] *= scale;
  invalidate();
}

void Transform::scale(const Vector3D & scale) {
  m_scale *= scale;
  invalidate();
}

void Transform::invalidate() {
  m_outdatedTransform = true;
}

void Transform::update() {
  m_outdatedTransform = false;
  m_transform.identity();
  m_transform.Rotation(m_rotation.x, m_rotation.y, m_rotation.z);
  m_transform.Translation(m_position);
  m_transform.Scale(m_scale);
}

}