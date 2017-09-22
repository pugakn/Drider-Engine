#include "dr_vector2d.h"

namespace driderSDK
{

Vector2D::Vector2D()
{
}

Vector2D::Vector2D(FORCE_INIT k)
{
  if (k == FORCE_INIT::kIdentity) {
    x = 0.0f;
    y = 0.0f;
  }
  else {
    x = 1.0f;
    y = 1.0f;
  }
}

Vector2D::Vector2D(const Vector2D& V)
  : x(V.x),
    y(V.y)
{
}

Vector2D::Vector2D(Float32 x,
                   Float32 y)
  : x(x),
    y(y)
{
}

Vector2D::~Vector2D()
{
}

Float32
Vector2D::dot(const Vector2D& B) const
{
  return (x*B.x) + (y*B.y);
}

Float32
Vector2D::length() const
{
  return math::sqrt(dot(*this));
}

Float32
Vector2D::lengthSqr() const
{
  return dot(*this);
}

Vector2D
Vector2D::normalize() const
{
  return (*this) * (1 / length());
}

Float32
Vector2D::distance(const Vector2D& S) const
{
  return (S - *this).length();
}

Float32
Vector2D::distanceSqr(const Vector2D& S) const
{
  return (S - *this).lengthSqr();
}

Float32&
Vector2D::operator[](SizeT index)
{
  if (index == 0) {
    return x;
  }
  else if (index == 1) {
    return y;
  }

  throw std::out_of_range("Index out of range");
}

const Float32&
Vector2D::operator[](SizeT index) const
{
  if (index == 0) {
    return x;
  }
  else if (index == 1) {
    return y;
  }

  throw std::out_of_range("Index out of range");
}

Vector2D&
Vector2D::operator=(const Vector2D& A)
{
  x = A.x;
  y = A.y;
  return *this;
}

Vector2D
Vector2D::operator+(const Vector2D& A) const
{
  return Vector2D(x + A.x, y + A.y);
}

Vector2D&
Vector2D::operator+=(const Vector2D& A)
{
  x += A.x;
  y += A.y;
  return *this;
}

Vector2D
Vector2D::operator-(const Vector2D& A) const
{
  return Vector2D(x - A.x, y - A.y);
}

Vector2D&
Vector2D::operator-=(const Vector2D& A)
{
  x -= A.x;
  y -= A.y;
  return *this;
}

Vector2D
Vector2D::operator*(const Vector2D& A) const
{
  return Vector2D(x*A.x, y*A.y);
}

Vector2D&
Vector2D::operator*=(const Vector2D& A)
{
  x *= A.x;
  y *= A.y;
  return *this;
}

Vector2D
Vector2D::operator*(const Float32 S) const
{
  return Vector2D(x*S, y*S);
}

Vector2D&
Vector2D::operator*=(const Float32 S)
{
  x *= S;
  y *= S;
  return *this;
}

Vector2D
Vector2D::operator/(const Float32 S) const
{
  return Vector2D(x / S, y / S);
}

Vector2D&
Vector2D::operator/=(const Float32 S)
{
  x /= S;
  y /= S;
  return *this;
}

bool
Vector2D::operator==(const Vector2D& S)
{
  return (x == S.x) && (y == S.y);
}

bool
Vector2D::operator!=(const Vector2D& S)
{
  return !((*this) == S);
}

}