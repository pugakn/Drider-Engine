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

  if(m_outdatedTransform){
      update();
  }
  
  return m_transform;
}

const Vector3D & Transform::getPosition() {
  return m_position;
}

const Vector3D & Transform::getRotation() {
  return m_rotation;
}

const Vector3D & Transform::getScale() {
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

void Transform::setRotation(const Vector3D & orientation) {
  m_rotation = orientation;
  invalidate();
}

void Transform::rotate(Radian angle, AXIS::E axis) {
  m_rotation[axis] += angle;
}

void Transform::rotate(Degree angle, AXIS::E axis) {
  m_rotation[axis] += angle.toRadian();
}

void Transform::rotate(const Vector3D & orientation) {
  m_rotation += orientation;
}

void Transform::setScale(float scale, AXIS::E axis) {
  m_scale[axis] = scale;
}

void Transform::setScale(const Vector3D& scale) {
  m_scale = scale;
}

void Transform::scale(float scale, AXIS::E axis) {
  m_scale[axis] *= scale;
}

void Transform::scale(const Vector3D & scale) {
  m_scale *= scale;
}

void Transform::invalidate() {
  m_outdatedTransform = true;
}

void Transform::update() {
  //TODO: Transform m_transform
  
  m_outdatedTransform = false;
}

}