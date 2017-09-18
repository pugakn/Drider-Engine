#include "dr_vector4d.h"

driderSDK::Vector4D::Vector4D()
  : x(0.0f),
    y(0.0f),
    z(0.0f) {
}

driderSDK::Vector4D::Vector4D(const Vector4D& V)
  : x(V.x),
    y(V.y),
    z(V.z) {
}

driderSDK::Vector4D::Vector4D(driderSDK::Float32 x,
                              driderSDK::Float32 y,
                              driderSDK::Float32 z,
                              driderSDK::Float32 w)
  : x(x),
    y(y),
    z(z),
    w(w) {
}

driderSDK::Vector4D::~Vector4D() {
}

driderSDK::Float32
driderSDK::Vector4D::dot(const Vector4D& B) const {
  return (x*B.x) + (y*B.y) + (z*B.z) + (w*B.w);
}

driderSDK::Vector4D
driderSDK::Vector4D::cross(const Vector4D& B) const {
  return Vector4D(y*B.z - z*B.y, z*B.x - x*B.z, x*B.y - y*B.x, 0.0f);
}

driderSDK::Float32
driderSDK::Vector4D::magnitude() const {
  return sqrt(dot(*this));
}

driderSDK::Vector4D
driderSDK::Vector4D::normalize() const {
  return (*this) * (1 / magnitude());
}

driderSDK::Vector4D
driderSDK::Vector4D::operator+(const Vector4D& A) const {
  return Vector4D(x + A.x, y + A.y, z + A.z, w + A.w);
}

driderSDK::Vector4D&
driderSDK::Vector4D::operator+=(const Vector4D& A) {
  x += A.x;
  y += A.y;
  z += A.z;
  w += A.w;
  return *this;
}

driderSDK::Vector4D
driderSDK::Vector4D::operator-(const Vector4D& A) const {
  return Vector4D(x - A.x, y - A.y, z - A.z, w - A.w);
}

driderSDK::Vector4D&
driderSDK::Vector4D::operator-=(const Vector4D& A) {
  x -= A.x;
  y -= A.y;
  z -= A.z;
  w -= A.w;
  return *this;
}

driderSDK::Vector4D
driderSDK::Vector4D::operator*(const Vector4D& A) const {
  return Vector4D(x*A.x, y*A.y, z*A.z, w * A.w);
}

driderSDK::Vector4D&
driderSDK::Vector4D::operator*=(const Vector4D& A) {
  x *= A.x;
  y *= A.y;
  z *= A.z;
  w *= A.w;
  return *this;
}

driderSDK::Vector4D
driderSDK::Vector4D::operator*(const Float32 S) const {
  return Vector4D(x*S, y*S, z*S, w*S);
}

driderSDK::Vector4D&
driderSDK::Vector4D::operator*=(const Float32 S) {
  x *= S;
  y *= S;
  z *= S;
  w *= S;
  return *this;
}

driderSDK::Vector4D
driderSDK::Vector4D::operator/(const Float32 S) const {
  return Vector4D(x*pow(S, -1), y*pow(S, -1), z*pow(S, -1), w*pow(S, -1));
}

driderSDK::Vector4D&
driderSDK::Vector4D::operator/=(const Float32 S) {
  x *= pow(S, -1);
  y *= pow(S, -1);
  z *= pow(S, -1);
  w *= pow(S, -1);
  return *this;
}