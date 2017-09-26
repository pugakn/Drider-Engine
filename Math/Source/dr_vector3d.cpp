#include "dr_vector3d.h"

namespace driderSDK {

Vector3D::Vector3D() : x(0.0f), y(0.0f), z(0.0f) {}

Vector3D::Vector3D(FORCE_INIT k) {
  if (FORCE_INIT::kIdentity == k) {
    x = 0.0f;
    y = 0.0f;
    z = 0.0f;
  }
  else {
    x = 1.0f;
    y = 1.0f;
    z = 1.0f;
  }
}

Vector3D::Vector3D(const Vector3D& V) : x(V.x), y(V.y), z(V.z) {}

Vector3D::Vector3D(float x, float y, float z) : x(x), y(y), z(z) {}

Vector3D::~Vector3D() {}

float
Vector3D::dot(const Vector3D& B) const {
  return (x*B.x) + (y*B.y) + (z*B.z);
}

Vector3D
Vector3D::cross(const Vector3D& B) const {
  return Vector3D(y*B.z - z*B.y, z*B.x - x*B.z, x*B.y - y*B.x);
}

float
Vector3D::length() const {
  return math::sqrt(dot(*this));
}

float
Vector3D::lengthSqr() const {
  return dot(*this);
}

Vector3D
Vector3D::normalize() const {
  return (*this) * (1 / length());
}

float
Vector3D::distance(const Vector3D& S) const {
  return (S - *this).length();
}

float
Vector3D::distanceSqr(const Vector3D& S) const {
  return (S - *this).lengthSqr();
}

float&
Vector3D::operator[](SizeT index) {
  assert(index >= 0 && index < 3);
  return data[index];
}

const float&
Vector3D::operator[](SizeT index) const {
  assert(index >= 0 && index < 3);
  return data[index];
}

Vector3D&
Vector3D::operator=(const Vector3D& A) {
  x = A.x;
  y = A.y;
  z = A.z;
  return *this;
}

Vector3D
Vector3D::operator+(const Vector3D& A) const {
  return Vector3D(x + A.x, y + A.y, z + A.z);
}

Vector3D&
Vector3D::operator+=(const Vector3D& A) {
  x += A.x;
  y += A.y;
  z += A.z;
  return *this;
}

Vector3D
Vector3D::operator-(const Vector3D& A) const {
  return Vector3D(x - A.x, y - A.y, z - A.z);
}

Vector3D&
Vector3D::operator-=(const Vector3D& A) {
  x -= A.x;
  y -= A.y;
  z -= A.z;
  return *this;
}

Vector3D
Vector3D::operator*(const Vector3D& A) const {
  return Vector3D(x*A.x, y*A.y, z*A.z);
}

Vector3D&
Vector3D::operator*=(const Vector3D& A) {
  x *= A.x;
  y *= A.y;
  z *= A.z;
  return *this;
}

Vector3D
Vector3D::operator*(const float S) const {
  return Vector3D(x*S, y*S, z*S);
}

Vector3D&
Vector3D::operator*=(const float S) {
  x *= S;
  y *= S;
  z *= S;
  return *this;
}

Vector3D
Vector3D::operator/(const float S) const {
  float invDiv = 1 / S;
  return Vector3D(x*invDiv, y*invDiv, z*invDiv);
}

Vector3D&
Vector3D::operator/=(const float S) {
  float invDiv = 1 / S;
  x *= invDiv;
  y *= invDiv;
  z *= invDiv;
  return *this;
}

bool
Vector3D::operator==(const Vector3D& S) {
  return ((x == S.x) && (y == S.y) && (z == S.z));
}

bool
Vector3D::operator!=(const Vector3D& S) {
  return !((*this) == S);
}

Vector3D
Vector3D::operator-() const {
  return Vector3D(-x, -y, -z);
}

}