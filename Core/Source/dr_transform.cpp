#include "dr_transform.h"
#include <dr_degree.h>
#include <dr_matrix3x3.h>
#include <dr_radian.h>
#include <dr_vector4d.h>

namespace driderSDK {

Transform::Transform() 
  : m_outdatedTransform(true),
    m_outdatedRotation(true),
    m_change(false),
    m_position(0,0,0),
    m_scale(1,1,1),
    m_rotX(Math::FORCE_INIT::kIdentity),
    m_rotY(Math::FORCE_INIT::kIdentity),
    m_rotZ(Math::FORCE_INIT::kIdentity),
    m_eulerAngles(0,0,0)
{}

Transform::Transform(const Transform& other)
{
  *this = other;
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
Transform::getEulerAngles() const {
  
  getMatrix(); //Update if needed.

  return m_eulerAngles;
}

const Vector3D& 
Transform::getDirection() const {

  getMatrix();

  return m_direction;
}

const Matrix4x4& 
Transform::getRotation() const {
  if (m_outdatedRotation) {
    
    m_rotation = m_rotZ * m_rotX * m_rotY;

    //Update euler angles
    m_eulerAngles = m_rotation.eulerAngles();

    //Update local direction

    m_direction = {Math::cos(m_eulerAngles.y) * Math::sin(m_eulerAngles.x),
                   Math::sin(m_eulerAngles.y),
                   Math::cos(m_eulerAngles.y) * Math::cos(m_eulerAngles.x)};

    m_direction.normalize();

    m_outdatedRotation = false;
  }

  return m_rotation;
}

const Vector3D&
Transform::getScale() const {
  return m_scale;
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
  m_position.data[axis] += dist;
  invalidate();
}

void
Transform::move(const Vector3D& distance) {
  m_position += distance;
  invalidate();
}

void
Transform::setRotation(Radian angle, AXIS::E axis) {

  switch (axis) {
  case driderSDK::AXIS::kX:
  m_rotX.identity().RotationX(angle);
  break;
  case driderSDK::AXIS::kY:
  m_rotY.identity().RotationY(angle);
  break;
  case driderSDK::AXIS::kZ:
  m_rotZ.identity().RotationZ(angle);
  break;
  }
  invalidateRotation();
}

void
Transform::setRotation(Degree angle, AXIS::E axis) {
  setRotation(Radian(angle.toRadian()), axis);
  invalidateRotation();
}

void
Transform::setRotation(const Vector3D& orientation) {
  m_rotX = m_rotY = m_rotZ = Matrix4x4::identityMat4x4;
  m_rotX.RotationX(orientation.x);
  m_rotY.RotationY(orientation.y);
  m_rotZ.RotationZ(orientation.z);
  invalidateRotation();
}

void
Transform::rotate(const Vector3D & orientation) {
  m_rotX.RotationX(orientation.x);
  m_rotY.RotationY(orientation.y);
  m_rotZ.RotationZ(orientation.z);
  invalidateRotation();
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
  
  r.m_position = Vector4D(other.m_scale * m_position, 1) * 
                 other.getRotation() + 
                 other.m_position;

  r.m_rotX = m_rotX * other.m_rotX;
  r.m_rotY = m_rotY * other.m_rotY;
  r.m_rotZ = m_rotZ * other.m_rotZ;

  r.m_scale = m_scale * other.m_scale;

  r.invalidateRotation();
  
  return r;
}

Transform Transform::operator*(const Matrix4x4& mat) const {
  
  Transform t;

  Vector3D otherPos = Vector3D(mat.vector3);

  Matrix3x3 temp(mat);

  temp.transpose();

  Vector3D otherScl(temp.vector0.length(), 
                    temp.vector1.length(), 
                    temp.vector2.length());
  
  for (Int32 i = 0; i < 3; ++i) {
    temp[i] /= otherScl[i];
  }

  temp.transpose();

  Matrix4x4 otherRot(temp);

  Vector3D x = otherScl * m_position;

  t.m_position = Vector4D(x,1) * otherRot + otherPos;

  t.m_rotX = m_rotX;
  t.m_rotY = m_rotY;
  t.m_rotZ = m_rotZ;

  auto euler = otherRot.eulerAngles();

  t.m_rotX.RotationX(euler.x);
  t.m_rotY.RotationY(euler.y);
  t.m_rotZ.RotationZ(euler.z);
  
  t.m_scale = m_scale * otherScl;

  t.invalidateRotation();

  return t;
}

Transform&
Transform::operator=(const Transform& other) {
  
  m_position = other.m_position;
  m_scale = other.m_scale;
  m_rotX = other.m_rotX;
  m_rotY = other.m_rotY;
  m_rotZ = other.m_rotZ;
  
  invalidateRotation();

  return *this;
}

void
Transform::invalidate() {
  m_change = true;
  m_outdatedTransform = true;
}

void 
Transform::invalidateRotation() {
  invalidate();
  m_outdatedRotation = true;
}

void
Transform::update() const {
  m_outdatedTransform = false;
  m_transform = getRotation();
  m_transform.Scale(m_scale);
  m_transform.Translation(m_position);
}

bool Transform::changed() const {
  return m_change;
}

}