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
Vector2D::distance(const Vector2D& S) const {
  return (S - *this).length();
}

float
Vector2D::distanceSqr(const Vector2D& S) const {
  return (S - *this).lengthSqr();
}

float&
Vector2D::operator[](SizeT index) {
  assert(index >= 0 && index < 2);
  return data[index];
}

const float&
Vector2D::operator[](SizeT index) const {
  assert(index >= 0 && index < 2);
  return data[index];
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
Vector2D::operator*(const float S) const {
  return Vector2D(x*S, y*S);
}

Vector2D&
Vector2D::operator*=(const float S) {
  x *= S;
  y *= S;
  return *this;
}

Vector2D
Vector2D::operator/(const float S) const {
  return Vector2D(x / S, y / S);
}

Vector2D&
Vector2D::operator/=(const float S) {
  x /= S;
  y /= S;
  return *this;
}

bool
Vector2D::operator==(const Vector2D& S) {
  return (x == S.x) && (y == S.y);
}

bool
Vector2D::operator!=(const Vector2D& S) {
  return !((*this) == S);
}

Vector2D
Vector2D::operator-() const {
  return Vector2D(-x, -y);
}

}