#include "dr_vector2di.h"

namespace driderSDK
{

Vector2DI::Vector2DI() {}

Vector2DI::Vector2DI(Math::FORCE_INIT k) {
  if (Math::FORCE_INIT::kZero == k) {
    x = 0;
    y = 0;
  }
  else {
    x = 1;
    y = 1;
  }
}

Vector2DI::Vector2DI(const Vector2DI& V) : x(V.x), y(V.y) {}

Vector2DI::Vector2DI(Int32 _x, Int32 _y) : x(_x), y(_y) {}

Vector2DI::~Vector2DI() {}

float
Vector2DI::dot(const Vector2DI& B) const {
  return static_cast<float>((x*B.x) + (y*B.y));
}

Vector2DI
Vector2DI::cross() const {
  return Vector2DI(x*y - y*x, 0);
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
  *this = (*this) * Math::pow(length(), -1.0f);
}

float
Vector2DI::distance(const Vector2DI& otherVector) const {
  return (otherVector - *this).length();
}

float
Vector2DI::distanceSqr(const Vector2DI& otherVector) const {
  return (otherVector - *this).lengthSqr();
}

bool
Vector2DI::equals(const Vector2DI& otherVector, float errorRange) const {
  return (Math::abs(x - otherVector.x) < errorRange) &&
         (Math::abs(y - otherVector.y) < errorRange);
}

Int32*
Vector2DI::ptr() {
  return &data[0];
}

const Int32*
Vector2DI::ptr() const {
  return &data[0];
}

Int32&
Vector2DI::operator[](const SizeT index) {
  DR_ASSERT(index < 2);
  return data[index];
}

const Int32&
Vector2DI::operator[](const SizeT index) const {
  DR_ASSERT(index < 2);
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
  return Vector2DI(static_cast<Int32>(x*Math::pow(scalar, -1.0f)),
                   static_cast<Int32>(y*Math::pow(scalar, -1.0f)));
}

Vector2DI&
Vector2DI::operator/=(const float scalar) {
  x *= static_cast<Int32>(Math::pow(scalar, -1.0f));
  y *= static_cast<Int32>(Math::pow(scalar, -1.0f));
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