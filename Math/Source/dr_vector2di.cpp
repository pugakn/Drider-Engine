#include "dr_vector2di.h"

namespace driderSDK
{

Vector2DI::Vector2DI() {}

Vector2DI::Vector2DI(Math::FORCE_INIT k) {
  if (k == Math::FORCE_INIT::kIdentity) {
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
  return Math::sqrt(dot(*this));
}

float
Vector2DI::lengthSqr() const {
  return dot(*this);
}

void
Vector2DI::normalize() {
  *this = (*this) * (1 / length());
}

float
Vector2DI::distance(const Vector2DI& otherVector) const {
  return (otherVector - *this).length();
}

float
Vector2DI::distanceSqr(const Vector2DI& otherVector) const {
  return (otherVector - *this).lengthSqr();
}

Int32&
Vector2DI::operator[](SizeT index) {
  DR_ASSERT(index >= 0 && index < 2);
  return data[index];
}

const Int32&
Vector2DI::operator[](SizeT index) const {
  DR_ASSERT(index >= 0 && index < 2);
  return data[index];
}

float
Vector2DI::operator|(const Vector2DI& B) const {
  return dot(B);
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
Vector2DI::operator*(const float scalar) const {
  return Vector2DI(static_cast<Int32>(x*scalar), static_cast<Int32>(y*scalar));
}

Vector2DI&
Vector2DI::operator*=(const float scalar) {
  x *= static_cast<Int32>(scalar);
  y *= static_cast<Int32>(scalar);
  return *this;
}

Vector2DI
Vector2DI::operator/(const float scalar) const {
  return Vector2DI(static_cast<Int32>(x*(1 / scalar)),
                   static_cast<Int32>(y*(1 / scalar)));
}

Vector2DI&
Vector2DI::operator/=(const float scalar) {
  x *= static_cast<Int32>(1 / scalar);
  y *= static_cast<Int32>(1 / scalar);
  return *this;
}

bool
Vector2DI::operator==(const Vector2DI& otherVector) {
  return ((x == otherVector.x) && (y == otherVector.y));
}

bool
Vector2DI::operator!=(const Vector2DI& otherVector) {
  return !((*this) == otherVector);
}

Vector2DI
Vector2DI::operator-() const {
  return Vector2DI(-x, -y);
}

}