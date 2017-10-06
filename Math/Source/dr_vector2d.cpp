#include "dr_vector2d.h"

namespace driderSDK
{

Vector2D::Vector2D() {}

Vector2D::Vector2D(Math::FORCE_INIT k) {
  if (k == Math::FORCE_INIT::kIdentity) {
    x = 0.0f;
    y = 0.0f;
  }
  else {
    x = 1.0f;
    y = 1.0f;
  }
}

Vector2D::Vector2D(const Vector2D& V) : x(V.x), y(V.y) {}

Vector2D::Vector2D(float x, float y) : x(x), y(y) {}

Vector2D::~Vector2D() {}

float
Vector2D::dot(const Vector2D& B) const {
  return (x*B.x) + (y*B.y);
}

float
Vector2D::length() const {
  return Math::sqrt(dot(*this));
}

float
Vector2D::lengthSqr() const {
  return dot(*this);
}

void
Vector2D::normalize() {
  *this = (*this) * (1 / length());
}

float
Vector2D::distance(const Vector2D& otherVector) const {
  return (otherVector - *this).length();
}

float
Vector2D::distanceSqr(const Vector2D& otherVector) const {
  return (otherVector - *this).lengthSqr();
}

float&
Vector2D::operator[](SizeT index) {
  DR_ASSERT(index >= 0 && index < 2);
  return data[index];
}

const float&
Vector2D::operator[](SizeT index) const {
  DR_ASSERT(index >= 0 && index < 2);
  return data[index];
}

float
Vector2D::operator|(const Vector2D& B) const {
  return dot(B);
}


Vector2D&
Vector2D::operator=(const Vector2D& A) {
  x = A.x;
  y = A.y;
  return *this;
}

Vector2D
Vector2D::operator+(const Vector2D& A) const {
  return Vector2D(x + A.x, y + A.y);
}

Vector2D&
Vector2D::operator+=(const Vector2D& A) {
  x += A.x;
  y += A.y;
  return *this;
}

Vector2D
Vector2D::operator-(const Vector2D& A) const {
  return Vector2D(x - A.x, y - A.y);
}

Vector2D&
Vector2D::operator-=(const Vector2D& A) {
  x -= A.x;
  y -= A.y;
  return *this;
}

Vector2D
Vector2D::operator*(const Vector2D& A) const {
  return Vector2D(x*A.x, y*A.y);
}

Vector2D&
Vector2D::operator*=(const Vector2D& A) {
  x *= A.x;
  y *= A.y;
  return *this;
}

Vector2D
Vector2D::operator*(const float scalar) const {
  return Vector2D(x*scalar, y*scalar);
}

Vector2D&
Vector2D::operator*=(const float scalar) {
  x *= scalar;
  y *= scalar;
  return *this;
}

Vector2D
Vector2D::operator/(const float scalar) const {
  return Vector2D(x / scalar, y / scalar);
}

Vector2D&
Vector2D::operator/=(const float scalar) {
  x /= scalar;
  y /= scalar;
  return *this;
}

bool
Vector2D::operator==(const Vector2D& otherVector) {
  return (x == otherVector.x) && (y == otherVector.y);
}

bool
Vector2D::operator!=(const Vector2D& otherVector) {
  return !((*this) == otherVector);
}

Vector2D
Vector2D::operator-() const {
  return Vector2D(-x, -y);
}

}