#include "dr_vector4d.h"

namespace driderSDK
{

Vector4D::Vector4D() {}

Vector4D::Vector4D(math::FORCE_INIT k) {
  if (math::FORCE_INIT::kIdentity == k) {
    x = 0.0f;
    y = 0.0f;
    z = 0.0f;
    w = 0.0f;
  }
  else {
    x = 1.0f;
    y = 1.0f;
    z = 1.0f;
    w = 1.0f;
  }
}

Vector4D::Vector4D(const Vector4D& V) : x(V.x), y(V.y), z(V.z), w(V.w) {}

Vector4D::Vector4D(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}

Vector4D::~Vector4D() {}

float
Vector4D::dot(const Vector4D& B) const {
  return (x*B.x) + (y*B.y) + (z*B.z) + (w*B.w);
}

Vector4D
Vector4D::cross(const Vector4D& B) const {
  return Vector4D(y*B.z - z*B.y, z*B.x - x*B.z, x*B.y - y*B.x, 0.0f);
}

float
Vector4D::length() const {
  return math::sqrt(dot(*this));
}

float
Vector4D::lengthSqr() const {
  return dot(*this);
}

Vector4D
Vector4D::normalize() const {
  return (*this) * (1 / length());
}

float
Vector4D::distance(const Vector4D& S) const {
  return (S - *this).length();
}

float
Vector4D::distanceSqr(const Vector4D & S) const {
  return (S - *this).lengthSqr();
}

float&
Vector4D::operator[](SizeT index) {
  assert(index >= 0 && index < 4);
  return data[index];
}

const float&
Vector4D::operator[](SizeT index) const {
  assert(index >= 0 && index < 4);
  return data[index];
}

Vector4D&
Vector4D::operator=(const Vector4D& A) {
  x = A.x;
  y = A.y;
  z = A.z;
  w = A.w;
  return *this;
}

Vector4D
Vector4D::operator+(const Vector4D& A) const {
  return Vector4D(x + A.x, y + A.y, z + A.z, w + A.w);
}

Vector4D&
Vector4D::operator+=(const Vector4D& A) {
  x += A.x;
  y += A.y;
  z += A.z;
  w += A.w;
  return *this;
}

Vector4D
Vector4D::operator-(const Vector4D& A) const {
  return Vector4D(x - A.x, y - A.y, z - A.z, w - A.w);
}

Vector4D&
Vector4D::operator-=(const Vector4D& A) {
  x -= A.x;
  y -= A.y;
  z -= A.z;
  w -= A.w;
  return *this;
}

Vector4D
Vector4D::operator*(const Vector4D& A) const {
  return Vector4D(x*A.x, y*A.y, z*A.z, w * A.w);
}

Vector4D&
Vector4D::operator*=(const Vector4D& A) {
  x *= A.x;
  y *= A.y;
  z *= A.z;
  w *= A.w;
  return *this;
}

Vector4D
Vector4D::operator*(const float S) const {
  return Vector4D(x*S, y*S, z*S, w*S);
}

Vector4D&
Vector4D::operator*=(const float S) {
  x *= S;
  y *= S;
  z *= S;
  w *= S;
  return *this;
}

Vector4D
Vector4D::operator/(const float S) const {
  float invDiv = 1 / S;
  return Vector4D(x*invDiv, y*invDiv, z*invDiv, w*invDiv);
}

Vector4D&
Vector4D::operator/=(const float S) {
  float invDiv = 1 / S;
  x *= invDiv;
  y *= invDiv;
  z *= invDiv;
  w *= invDiv;
  return *this;
}

bool
Vector4D::operator==(const Vector4D& S)  {
  return ((x == S.x) && (y == S.y) && (z == S.z) && (w == S.w));
}

bool
Vector4D::operator!=(const Vector4D& S) {
  return !((*this) == S);
}

Vector4D
Vector4D::operator-() const {
  return Vector4D(x-, -y, -z, -w);
}

}