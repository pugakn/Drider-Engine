#include "dr_vector2d.h"

namespace driderSDK
{

Vector2D::Vector2D() {}

Vector2D::Vector2D(Math::FORCE_INIT k) {
  if (Math::FORCE_INIT::kZero == k) {
    x = 0.0f;
    y = 0.0f;
  }
  else {
    x = 1.0f;
    y = 1.0f;
  }
}

Vector2D::Vector2D(const Vector2D& V) : x(V.x), y(V.y) {}

Vector2D::Vector2D(float _x, float _y) : x(_x), y(_y) {}

Vector2D::~Vector2D() {}

float
Vector2D::dot(const Vector2D& B) const {
  return (x*B.x) + (y*B.y);
}

Vector2D
Vector2D::cross() const {
  return Vector2D(x, -y);
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
  DR_ASSERT(length() != 0.0f);
  *this /= length();
}

float
Vector2D::distance(const Vector2D& otherVector) const {
  return (otherVector - *this).length();
}

float
Vector2D::distanceSqr(const Vector2D& otherVector) const {
  return (otherVector - *this).lengthSqr();
}

bool
Vector2D::equals(const Vector2D& otherVector, float errorRange) const {
  return (Math::abs(x - otherVector.x) < errorRange) &&
         (Math::abs(y - otherVector.y) < errorRange);
}

Int32
Vector2D::compare(const Vector2D& p1, const Vector2D& p2) {
  Int32 compOrientation = orientation(p1, p2);

  if (compOrientation == 0) {
    if (distanceSqr(p1) < distanceSqr(p2)) {
      return 1;
    }
    else {
      return -1;
    }
  }

  if (compOrientation == 2) {
    return -1;
  }
  else {
    return 1;
  }
}

Int32
Vector2D::orientation(const Vector2D& q, const Vector2D& r) const {
  float val = (q.y - y) * (r.x - q.x) - (q.x - x) * (r.y - q.y);

  if (val == 0.0f) {
    return 0;  // colinear
  }
  else if (val > 0.0f) {
    return 1;
  }
  else {
    return 2; // clock or counterclock wise
  }
}

float*
Vector2D::ptr() {
  return &data[0];
}

const float*
Vector2D::ptr() const {
  return &data[0];
}

float&
Vector2D::operator[](const SizeT index){
  DR_ASSERT(index < 2);
  return data[index];
}

const float&
Vector2D::operator[](const SizeT index) const {
  DR_ASSERT(index < 2);
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
  DR_ASSERT(scalar != 0.0f);
  return Vector2D(x / scalar, y / scalar);
}

Vector2D&
Vector2D::operator/=(const float scalar) {
  DR_ASSERT(scalar != 0.0f);
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