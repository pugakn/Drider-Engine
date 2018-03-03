#include "dr_transform.h"
#include <dr_degree.h>
#include <dr_radian.h>
#include <dr_vector4d.h>

namespace driderSDK {

Transform::Transform() 
  : m_outdatedTransform(true),
    m_position(0,0,0),
    m_scale(1,1,1),
    m_rotation(0,0,0),
    m_changed(false) {
}

const Matrix4x4& 
Transform::getMatrix() const {  

  if (m_outdatedTransform) {
    update();
  }

  return m_transform;
}

const Vector3D& 
Transform::getPosition() const {
  return m_position;
}

const Vector3D&
Transform::getRotation() const {
  return m_rotation;
}

const Vector3D&
Transform::getScale() const {
  return m_scale;
}

Vector3D Transform::getDirection() const {
  return Vector3D{Math::sin(-m_rotation.y),
                  Math::cos(m_rotation.y) * Math::sin(m_rotation.x),
                  Math::cos(m_rotation.y) * Math::cos(m_rotation.x)}.normalize();
}

void
Transform::setPosition(float pos, AXIS::E axis) {
  m_position[axis] = pos;
  invalidate();
}

void
Transform::setPosition(const Vector3D & position) {
  m_position = position;
  invalidate();
}

void
Transform::move(float dist, AXIS::E axis) {
  m_position[axis] += dist;
  invalidate();
}

void
Transform::move(const Vector3D& distance) {
  m_position += distance;
  invalidate();
}

void
Transform::setRotation(Radian angle, AXIS::E axis) {
  m_rotation[axis] = angle;
  invalidate();
}

void
Transform::setRotation(Degree angle, AXIS::E axis) {
  m_rotation[axis] = angle.toRadian();
  invalidate();
}

void
Transform::setRotation(const Vector3D& orientation) {
  m_rotation = orientation;
  invalidate();
}

void
Transform::rotate(Radian angle, AXIS::E axis) {
  m_rotation[axis] += angle;
    invalidate();
}

void
Transform::rotate(Degree angle, AXIS::E axis) {
  m_rotation[axis] += angle.toRadian();
  invalidate();
}

void
Transform::rotate(const Vector3D & orientation) {
  m_rotation += orientation;
  invalidate();
}

void
Transform::setScale(float scale, AXIS::E axis) {
  m_scale[axis] = scale;
  invalidate();
}

void
Transform::setScale(const Vector3D& scale) {
  m_scale = scale;
  invalidate();
}

void
Transform::scale(float scale, AXIS::E axis) {
  m_scale[axis] *= scale;
  invalidate();
}

void
Transform::scale(const Vector3D & scale) {
  m_scale *= scale;
  invalidate();
}

Transform Transform::operator*(const Transform& other) const {
  
  Transform r;
  Vector3D x = other.m_scale;
  x.x *= m_position.x;
  x.y *= m_position.y;
  x.z *= m_position.z;

  Matrix4x4 mat(Math::FORCE_INIT::kIdentity);
  mat.Rotation(other.m_rotation.x, other.m_rotation.y, other.m_rotation.z);

  r.m_position = Vector4D(x,1) * mat + other.m_position;
  r.m_rotation = m_rotation + other.m_rotation;
  r.m_scale = m_scale * other.m_scale;
  r.update();

  return r;
}

bool 
Transform::operator==(const Transform& other) const {
  return m_position == other.m_position && 
         m_rotation == other.m_rotation &&
         m_scale == other.m_scale;
}

bool Transform::operator!=(const Transform & other) const {
  return !(*this == other);
}

void
Transform::invalidate() {
  m_changed = true;
  m_outdatedTransform = true;
}

void
Transform::update() const {
  m_outdatedTransform = false;
  m_transform.identity();
  m_transform.Rotation(m_rotation.x, m_rotation.y, m_rotation.z);
  m_transform.Scale(m_scale);
  m_transform.Translation(m_position);
}

void 
Transform::newFrame() {
  m_changed = false;
}

bool Transform::changed() const {
  return m_changed;
}

}