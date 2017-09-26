#include "dr_vector2di.h"

namespace driderSDK
{

Vector2DI::Vector2DI()
{
}

Vector2DI::Vector2DI(FORCE_INIT k)
{
  if (k == FORCE_INIT::kIdentity) {
    x = 0;
    y = 0;
  }
  else {
    x = 1;
    y = 1;
  }
}

Vector2DI::Vector2DI(const Vector2DI& V)
  : x(V.x),
    y(V.y)
{
}

Vector2DI::Vector2DI(Int32 x,
                     Int32 y)
  : x(x),
    y(y)
{
}

Vector2DI::~Vector2DI()
{
}

float
Vector2DI::dot(const Vector2DI& B) const
{
  return static_cast<float>((x*B.x) + (y*B.y));
}

float
Vector2DI::length() const
{
  return math::sqrt(dot(*this));
}

float
Vector2DI::lengthSqr() const
{
  return dot(*this);
}

Vector2DI
Vector2DI::normalize() const
{
  return (*this) * (1 / length());
}

float
Vector2DI::distance(const Vector2DI& S) const
{
  return (S  - *this).length();
}

float
Vector2DI::distanceSqr(const Vector2DI& S) const
{
  return (S - *this).lengthSqr();
}

Int32&
Vector2DI::operator[](SizeT index)
{
  if (index == 0) {
    return x;
  }
  else if (index == 1) {
    return y;
  }
  throw std::out_of_range("Index out of range");
}

const Int32&
Vector2DI::operator[](SizeT index) const
{
  if (index == 0) {
    return x;
  }
  else if (index == 1) {
    return y;
  }
  throw std::out_of_range("Index out of range");
}

Vector2DI&
Vector2DI::operator=(const Vector2DI& A)
{
  x = A.x;
  y = A.y;
  return *this;
}

Vector2DI
Vector2DI::operator+(const Vector2DI& A) const
{
  return Vector2DI(x + A.x, y + A.y);
}

Vector2DI&
Vector2DI::operator+=(const Vector2DI& A)
{
  x += A.x;
  y += A.y;
  return *this;
}

Vector2DI
Vector2DI::operator-(const Vector2DI& A) const
{
  return Vector2DI(x - A.x, y - A.y);
}

Vector2DI&
Vector2DI::operator-=(const Vector2DI& A)
{
  x -= A.x;
  y -= A.y;
  return *this;
}

Vector2DI
Vector2DI::operator*(const Vector2DI& A) const
{
  return Vector2DI(x*A.x, y*A.y);
}

Vector2DI&
Vector2DI::operator*=(const Vector2DI& A)
{
  x *= A.x;
  y *= A.y;
  return *this;
}

Vector2DI
Vector2DI::operator*(const float S) const
{
  return Vector2DI(static_cast<Int32>(x*S),
                   static_cast<Int32>(y*S));
}

Vector2DI&
Vector2DI::operator*=(const float S)
{
  x *= static_cast<Int32>(S);
  y *= static_cast<Int32>(S);
  return *this;
}

Vector2DI
Vector2DI::operator/(const float S) const
{
  return Vector2DI(static_cast<Int32>(x*(1 / S)),
                   static_cast<Int32>(y*(1 / S)));
}

Vector2DI&
Vector2DI::operator/=(const float S)
{
  x *= static_cast<Int32>(1 / S);
  y *= static_cast<Int32>(1 / S);
  return *this;
}

bool
Vector2DI::operator==(const Vector2DI& S)
{
  return ((x == S.x) && (y == S.y));
}

bool
Vector2DI::operator!=(const Vector2DI& S)
{
  return !((*this) == S);
}

}