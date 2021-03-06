#include "dr_vector3d.h"
#include "dr_vector4d.h"

namespace driderSDK
{

CONSTRUCT_DESTRUCT_DEF(Vector3D)
COPY_CONSTRUCT_DEF(Vector3D)
CONSTRUCT_3P_DEF(Vector3D, float, float, float)

Vector3D::Vector3D(Math::FORCE_INIT k) {
  if (Math::FORCE_INIT::kZero == k) {
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

Vector3D::Vector3D(const Vector4D& V) : x(V.x), y(V.y), z(V.z) {}

Vector3D::Vector3D(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}

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
  return Math::sqrt(dot(*this));
}

float
Vector3D::lengthSqr() const {
  return dot(*this);
}

Vector3D&
Vector3D::normalize() {
  DR_ASSERT(length() != 0.0f); // No funciona xd, para el test
  *this /= length();
  return *this;
}

float
Vector3D::distance(const Vector3D& otherVector) const {
  return (otherVector - *this).length();
}

float
Vector3D::distanceSqr(const Vector3D& otherVector) const {
  return (otherVector - *this).lengthSqr();
}

bool
Vector3D::equals(const Vector3D& otherVector, float errorRange) const {
  return (Math::abs(x - otherVector.x) < errorRange) &&
         (Math::abs(y - otherVector.y) < errorRange) &&
         (Math::abs(z - otherVector.z) < errorRange);
}

float
Vector3D::sqrDistSegment(const Vector3D& pointA, const Vector3D& pointB) const {
  Vector3D AB = pointB - pointA;
  Vector3D AC = *this - pointA;
  Vector3D BC = *this - pointB;
  float e = AC.dot(AB);

  if (e <= 0.0f) {
   return AC.dot(AC);
  }

  float f = AB.dot(AB);
  
  if (e >= f) {
   return BC.dot(BC);
  }
  
  return AC.dot(AC) - (e * e / f);
}

float* Vector3D::ptr() {
  return &data[0];
}

const float* Vector3D::ptr() const {
  return &data[0];
}

float&
Vector3D::operator[](const SizeT index){
  DR_ASSERT(index < 3);
  return data[index];
}

const float&
Vector3D::operator[](const SizeT index) const {
  DR_ASSERT(index < 3);
  return data[index];
}

float
Vector3D::operator|(const Vector3D& B) const {
  return dot(B);
}

Vector3D
Vector3D::operator^(const Vector3D& B) const {
  return cross(B);
}

Vector3D&
Vector3D::operator=(const Vector3D& A) {
  x = A.x;
  y = A.y;
  z = A.z;
  return *this;
}

Vector3D&
Vector3D::operator=(const Vector4D& A) {
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
Vector3D::operator*(const float scalar) const {
  return Vector3D(x*scalar, y*scalar, z*scalar);
}

Vector3D&
Vector3D::operator*=(const float scalar) {
  x *= scalar;
  y *= scalar;
  z *= scalar;
  return *this;
}

Vector3D
Vector3D::operator/(const float scalar) const {
  DR_ASSERT(scalar != 0.0f);
  return Vector3D(x/ scalar, y/ scalar, z/ scalar);
}

Vector3D&
Vector3D::operator/=(const float scalar) {
  DR_ASSERT(scalar != 0.0f);
  x /= scalar;
  y /= scalar;
  z /= scalar;
  return *this;
}

bool
Vector3D::operator==(const Vector3D& otherVector) const {
  return ((x == otherVector.x) && (y == otherVector.y) && (z == otherVector.z));
}

bool
Vector3D::operator!=(const Vector3D& otherVector) const {
  return !((*this) == otherVector);
}

Vector3D
Vector3D::operator-() const {
  return Vector3D(-x, -y, -z);
}

}