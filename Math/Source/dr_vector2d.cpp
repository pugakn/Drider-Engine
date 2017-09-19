#include "dr_vector2d.h"

driderSDK::Vector2D::Vector2D()
  : x(0.0f),
    y(0.0f) {
}

driderSDK::Vector2D::Vector2D(const Vector2D& V)
  : x(V.x),
    y(V.y) {
}

driderSDK::Vector2D::Vector2D(Float32 x,
                              Float32 y)
  : x(x),
    y(y) {
}

driderSDK::Vector2D::~Vector2D() {
}

driderSDK::Float32
driderSDK::Vector2D::dot(const Vector2D& B) const {
  return (x*B.x) + (y*B.y);
}

driderSDK::Float32
driderSDK::Vector2D::magnitude() const {
  return sqrt(dot(*this));
}

driderSDK::Vector2D
driderSDK::Vector2D::normalize() const {
  return (*this) * (1 / magnitude());
}

driderSDK::Vector2D&
driderSDK::Vector2D::operator=(const Vector2D& A) {
  x = A.x;
  y = A.y;
  return *this;
}

driderSDK::Vector2D
driderSDK::Vector2D::operator+(const Vector2D& A) const {
  return Vector2D(x + A.x, y + A.y);
}

driderSDK::Vector2D&
driderSDK::Vector2D::operator+=(const Vector2D& A) {
  x += A.x;
  y += A.y;
  return *this;
}

driderSDK::Vector2D
driderSDK::Vector2D::operator-(const Vector2D& A) const {
  return Vector2D(x - A.x, y - A.y);
}

driderSDK::Vector2D&
driderSDK::Vector2D::operator-=(const Vector2D& A) {
  x -= A.x;
  y -= A.y;
  return *this;
}

driderSDK::Vector2D
driderSDK::Vector2D::operator*(const Vector2D& A) const {
  return Vector2D(x*A.x, y*A.y);
}

driderSDK::Vector2D&
driderSDK::Vector2D::operator*=(const Vector2D& A) {
  x *= A.x;
  y *= A.y;
  return *this;
}

driderSDK::Vector2D
driderSDK::Vector2D::operator*(const Float32 S) const {
  return Vector2D(x*S, y*S);
}

driderSDK::Vector2D&
driderSDK::Vector2D::operator*=(const Float32 S) {
  x *= S;
  y *= S;
  return *this;
}

driderSDK::Vector2D
driderSDK::Vector2D::operator/(const Float32 S) const {
  return Vector2D(x*(1 / S), y*(1 / S));
}

driderSDK::Vector2D&
driderSDK::Vector2D::operator/=(const Float32 S) {
  x *= (1 / S);
  y *= (1 / S);
  return *this;
}