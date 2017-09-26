#include "dr_vector2di.h"

namespace driderSDK
{

Vector2DI::Vector2DI() {}

Vector2DI::Vector2DI(math::FORCE_INIT k) {
  if (k == math::FORCE_INIT::kIdentity) {
    x = 0;
    y = 0;
  }
  else {
    x = 1;
    y = 1;
  }
}

Vector2DI::Vector2DI(const Vector2DI& V) : x(V.x), y(V.y) {}

Vector2DI::Vector2DI(Int32 x, Int32 y) : x(x), y(y) {}

Vector2DI::~Vector2DI() {}

float
Vector2DI::dot(const Vector2DI& B) const {
  return static_cast<float>((x*B.x) + (y*B.y));
}

float
Vector2DI::length() const {
  return math::sqrt(dot(*this));
}

float
Vector2DI::lengthSqr() const {
  return dot(*this);
}

Vector2DI
Vector2DI::normalize() const {
  return (*this) * (1 / length());
}

float
Vector2DI::distance(const Vector2DI& S) const {
  return (S  - *this).length();
}

float
Vector2DI::distanceSqr(const Vector2DI& S) const {
  return (S - *this).lengthSqr();
}

Int32&
Vector2DI::operator[](SizeT index) {
  assert(index >= 0 && index < 2);
  return data[index];
}

const Int32&
Vector2DI::operator[](SizeT index) const {
  assert(index >= 0 && index < 2);
  return data[index];
}

Vector2DI&
Vector2DI::operator=(const Vector2DI& A) {
  x = A.x;
  y = A.y;
  return *this;
}

Vector2DI
Vector2DI::operator+(const Vector2DI& A) const {
  return Vector2DI(x + A.x, y + A.y);
}

Vector2DI&
Vector2DI::operator+=(const Vector2DI& A) {
  x += A.x;
  y += A.y;
  return *this;
}

Vector2DI
Vector2DI::operator-(const Vector2DI& A) const {
  return Vector2DI(x - A.x, y - A.y);
}

Vector2DI&
Vector2DI::operator-=(const Vector2DI& A) {
  x -= A.x;
  y -= A.y;
  return *this;
}

Vector2DI
Vector2DI::operator*(const Vector2DI& A) const {
  return Vector2DI(x*A.x, y*A.y);
}

Vector2DI&
Vector2DI::operator*=(const Vector2DI& A) {
  x *= A.x;
  y *= A.y;
  return *this;
}

Vector2DI
Vector2DI::operator*(const float S) const {
  return Vector2DI(static_cast<Int32>(x*S), static_cast<Int32>(y*S));
}

Vector2DI&
Vector2DI::operator*=(const float S) {
  x *= static_cast<Int32>(S);
  y *= static_cast<Int32>(S);
  return *this;
}

Vector2DI
Vector2DI::operator/(const float S) const {
  return Vector2DI(static_cast<Int32>(x*(1 / S)),
                   static_cast<Int32>(y*(1 / S)));
}

Vector2DI&
Vector2DI::operator/=(const float S) {
  x *= static_cast<Int32>(1 / S);
  y *= static_cast<Int32>(1 / S);
  return *this;
}

bool
Vector2DI::operator==(const Vector2DI& S) {
  return ((x == S.x) && (y == S.y));
}

bool
Vector2DI::operator!=(const Vector2DI& S) {
  return !((*this) == S);
}

Vector2DI
Vector2DI::operator-() const {
  return Vector2DI(-x, -y);
}

}