#include "dr_vector4d.h"

namespace driderSDK
{

Vector4D::Vector4D()
  : x(0.0f),
    y(0.0f),
    z(0.0f),
    w(0.0f)
{
}

Vector4D::Vector4D(const Vector4D& V)
  : x(V.x),
    y(V.y),
    z(V.z),
    w(V.w)
{
}

Vector4D::Vector4D(Float32 x,
                   Float32 y,
                   Float32 z,
                   Float32 w)
  : x(x),
    y(y),
    z(z),
    w(w)
{
}

Vector4D::~Vector4D()
{
}

Float32
Vector4D::dot(const Vector4D& B) const
{
  return (x*B.x) + (y*B.y) + (z*B.z) + (w*B.w);
}

Vector4D
Vector4D::cross(const Vector4D& B) const
{
  return Vector4D(y*B.z - z*B.y, z*B.x - x*B.z, x*B.y - y*B.x, 0.0f);
}

Float32
Vector4D::length() const
{
  return sqrt(dot(*this));
}

Float32
Vector4D::lengthSqr() const
{
  return dot(*this);
}

Vector4D
Vector4D::normalize() const
{
  return (*this) * (1 / length());
}

Float32&
Vector4D::operator[](SizeT index)
{
  if (index == 0) {
    return x;
  }
  else if (index == 1) {
    return y;
  }
  else if (index == 2) {
    return z;
  }
  else if (index == 3) {
    return w;
  }
  throw std::out_of_range("Index out of range");
}

Vector4D&
Vector4D::operator=(const Vector4D& A)
{
  x = A.x;
  y = A.y;
  z = A.z;
  w = A.w;
  return *this;
}

Vector4D
Vector4D::operator+(const Vector4D& A) const
{
  return Vector4D(x + A.x, y + A.y, z + A.z, w + A.w);
}

Vector4D&
Vector4D::operator+=(const Vector4D& A)
{
  x += A.x;
  y += A.y;
  z += A.z;
  w += A.w;
  return *this;
}

Vector4D
Vector4D::operator-(const Vector4D& A) const
{
  return Vector4D(x - A.x, y - A.y, z - A.z, w - A.w);
}

Vector4D&
Vector4D::operator-=(const Vector4D& A)
{
  x -= A.x;
  y -= A.y;
  z -= A.z;
  w -= A.w;
  return *this;
}

Vector4D
Vector4D::operator*(const Vector4D& A) const
{
  return Vector4D(x*A.x, y*A.y, z*A.z, w * A.w);
}

Vector4D&
Vector4D::operator*=(const Vector4D& A)
{
  x *= A.x;
  y *= A.y;
  z *= A.z;
  w *= A.w;
  return *this;
}

Vector4D
Vector4D::operator*(const Float32 S) const
{
  return Vector4D(x*S, y*S, z*S, w*S);
}

Vector4D&
Vector4D::operator*=(const Float32 S)
{
  x *= S;
  y *= S;
  z *= S;
  w *= S;
  return *this;
}

Vector4D
Vector4D::operator/(const Float32 S) const
{
  return Vector4D(x*pow(S, -1), y*pow(S, -1), z*pow(S, -1), w*pow(S, -1));
}

Vector4D&
Vector4D::operator/=(const Float32 S)
{
  x *= pow(S, -1);
  y *= pow(S, -1);
  z *= pow(S, -1);
  w *= pow(S, -1);
  return *this;
}

}