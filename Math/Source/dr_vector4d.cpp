#include "dr_vector4d.h"
#include "dr_vector3d.h"

namespace driderSDK
{

Vector4D::Vector4D() {}

Vector4D::Vector4D(Math::FORCE_INIT k) {

  if (Math::FORCE_INIT::kZero == k) {
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

Vector4D::Vector4D(const Vector3D& V, float w) : x(V.x), y(V.y), z(V.z), w(w) {}

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
  return Math::sqrt(dot(*this));
}

float
Vector4D::lengthSqr() const {
  return dot(*this);
}

void
Vector4D::normalize() {
  *this = (*this) * Math::pow(length(), -1.0f);
}

float
Vector4D::distance(const Vector4D& otherVector) const {
  return (otherVector - *this).length();
}

float
Vector4D::distanceSqr(const Vector4D& otherVector) const {
  return (otherVector - *this).lengthSqr();
}

bool
Vector4D::equals(const Vector4D& otherVector) const {
  return (Math::abs(x - otherVector.x) < Math::SMALL_NUMBER) &&
         (Math::abs(y - otherVector.y) < Math::SMALL_NUMBER) &&
         (Math::abs(z - otherVector.z) < Math::SMALL_NUMBER) &&
         (Math::abs(w - otherVector.w) < Math::SMALL_NUMBER);
}

bool
Vector4D::equals(const Vector4D& otherVector, float errorRange) const {
  return (Math::abs(x - otherVector.x) < errorRange) &&
         (Math::abs(y - otherVector.y) < errorRange) &&
         (Math::abs(z - otherVector.z) < errorRange) &&
         (Math::abs(w - otherVector.w) < errorRange);
}

float&
Vector4D::operator[](SizeT index) {
  DR_ASSERT(index >= 0 && index < 4);
  return data[index];
}

const float&
Vector4D::operator[](SizeT index) const {
  DR_ASSERT(index >= 0 && index < 4);
  return data[index];
}

float
Vector4D::operator|(const Vector4D& B) const {
  return dot(B);
}

Vector4D
Vector4D::operator^(const Vector4D& B) const {
  return cross(B);
}

Vector4D&
Vector4D::operator=(const Vector4D& A) {
  x = A.x;
  y = A.y;
  z = A.z;
  w = A.w;
  return *this;
}

Vector4D&
Vector4D::operator=(const Vector3D& A) {
  x = A.x;
  y = A.y;
  z = A.z;
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
Vector4D::operator*(const float scalar) const {
  return Vector4D(x*scalar, y*scalar, z*scalar, w*scalar);
}

Vector4D&
Vector4D::operator*=(const float scalar) {
  x *= scalar;
  y *= scalar;
  z *= scalar;
  w *= scalar;
  return *this;
}

Vector4D
Vector4D::operator/(const float scalar) const {
  float invDiv = Math::pow(scalar, -1.0f);
  return Vector4D(x*invDiv, y*invDiv, z*invDiv, w*invDiv);
}

Vector4D&
Vector4D::operator/=(const float scalar) {
  float invDiv = Math::pow(scalar, -1.0f);
  x *= invDiv;
  y *= invDiv;
  z *= invDiv;
  w *= invDiv;
  return *this;
}

bool
Vector4D::operator==(const Vector4D& otherVector) {
  return ((x == otherVector.x) &&
          (y == otherVector.y) &&
          (z == otherVector.z) &&
          (w == otherVector.w));
}

bool
Vector4D::operator!=(const Vector4D& otherVector) {
  return !((*this) == otherVector);
}

Vector4D
Vector4D::operator-() const {
  return Vector4D(-x, -y, -z, -w);
}

}