#include "dr_vector3d.h"

driderSDK::Vector3D::Vector3D()
  : x(0.0f),
    y(0.0f),
    z(0.0f) {
}

driderSDK::Vector3D::Vector3D(const Vector3D& V)
  : x(V.x),
    y(V.y),
    z(V.z) {
}

driderSDK::Vector3D::Vector3D(driderSDK::Float32 x,
                              driderSDK::Float32 y,
                              driderSDK::Float32 z)
  : x(x),
    y(y),
    z(z) {
}

driderSDK::Vector3D::~Vector3D() {
}

driderSDK::Float32
driderSDK::Vector3D::dot(const Vector3D& B) const {
  return (x*B.x) + (y*B.y) + (z*B.z);
}

driderSDK::Vector3D
driderSDK::Vector3D::cross(const Vector3D& B) const {
  return Vector3D(y*B.z - z*B.y, z*B.x - x*B.z, x*B.y - y*B.x);
}

driderSDK::Float32
driderSDK::Vector3D::magnitude() const {
  return sqrt(dot(*this));
}

driderSDK::Vector3D
driderSDK::Vector3D::normalize() const {
  return (*this) * (1 / magnitude());
}

driderSDK::Vector3D
driderSDK::Vector3D::operator+(const Vector3D& A) const {
  return Vector3D(x + A.x, y + A.y, z + A.z);
}

driderSDK::Vector3D&
driderSDK::Vector3D::operator+=(const Vector3D& A) {
  x += A.x;
  y += A.y;
  z += A.z;
  return *this;
}

driderSDK::Vector3D
driderSDK::Vector3D::operator-(const Vector3D& A) const {
  return Vector3D(x - A.x, y - A.y, z - A.z);
}

driderSDK::Vector3D&
driderSDK::Vector3D::operator-=(const Vector3D& A) {
  x -= A.x;
  y -= A.y;
  z -= A.z;
  return *this;
}

driderSDK::Vector3D
driderSDK::Vector3D::operator*(const Vector3D& A) const {
  return Vector3D(x*A.x, y*A.y, z*A.z);
}

driderSDK::Vector3D&
driderSDK::Vector3D::operator*=(const Vector3D& A) {
  x *= A.x;
  y *= A.y;
  z *= A.z;
  return *this;
}

driderSDK::Vector3D
driderSDK::Vector3D::operator*(const Float32 S) const {
  return Vector3D(x*S, y*S, z*S);
}

driderSDK::Vector3D&
driderSDK::Vector3D::operator*=(const Float32 S) {
  x *= S;
  y *= S;
  z *= S;
  return *this;
}

driderSDK::Vector3D
driderSDK::Vector3D::operator/(const Float32 S) const {
  return Vector3D(x*pow(S, -1), y*pow(S, -1), z*pow(S, -1));
}

driderSDK::Vector3D&
driderSDK::Vector3D::operator/=(const Float32 S) {
  x *= pow(S, -1);
  y *= pow(S, -1);
  z *= pow(S, -1);
  return *this;
}