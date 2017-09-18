#include "dr_vector2di.h"

driderSDK::Vector2DI::Vector2DI()
  : x(0),
    y(0) {
}

driderSDK::Vector2DI::Vector2DI(const Vector2DI& V)
  : x(V.x),
    y(V.y) {
}

driderSDK::Vector2DI::Vector2DI(driderSDK::Int32 x,
                                driderSDK::Int32 y)
  : x(x),
    y(y) {
}

driderSDK::Vector2DI::~Vector2DI() {
}

driderSDK::Float32
driderSDK::Vector2DI::dot(const Vector2DI& B) const {
  return (x*B.x) + (y*B.y);
}

driderSDK::Float32
driderSDK::Vector2DI::magnitude() const {
  return sqrt(dot(*this));
}

driderSDK::Vector2DI
driderSDK::Vector2DI::normalize() const {
  return (*this) * (1 / magnitude());
}

driderSDK::Vector2DI
driderSDK::Vector2DI::operator+(const Vector2DI& A) const {
  return Vector2DI(x + A.x, y + A.y);
}

driderSDK::Vector2DI&
driderSDK::Vector2DI::operator+=(const Vector2DI& A) {
  x += A.x;
  y += A.y;
  return *this;
}

driderSDK::Vector2DI
driderSDK::Vector2DI::operator-(const Vector2DI& A) const {
  return Vector2DI(x - A.x, y - A.y);
}

driderSDK::Vector2DI&
driderSDK::Vector2DI::operator-=(const Vector2DI& A) {
  x -= A.x;
  y -= A.y;
  return *this;
}

driderSDK::Vector2DI
driderSDK::Vector2DI::operator*(const Vector2DI& A) const {
  return Vector2DI(x*A.x, y*A.y);
}

driderSDK::Vector2DI&
driderSDK::Vector2DI::operator*=(const Vector2DI& A) {
  x *= A.x;
  y *= A.y;
  return *this;
}

driderSDK::Vector2DI
driderSDK::Vector2DI::operator*(const Float32 S) const {
  return Vector2DI(x*S, y*S);
}

driderSDK::Vector2DI&
driderSDK::Vector2DI::operator*=(const Float32 S) {
  x *= S;
  y *= S;
  return *this;
}

driderSDK::Vector2DI
driderSDK::Vector2DI::operator/(const Float32 S) const {
  return Vector2DI(x*(1 / S), y*(1 / S));
}

driderSDK::Vector2DI&
driderSDK::Vector2DI::operator/=(const Float32 S) {
  x *= (1 / S);
  y *= (1 / S);
  return *this;
}