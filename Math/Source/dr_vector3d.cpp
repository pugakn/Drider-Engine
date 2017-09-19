#include "dr_vector3d.h"

namespace driderSDK
{

Vector3D::Vector3D()
  : x(0.0f),
    y(0.0f),
    z(0.0f)
{
}

Vector3D::Vector3D(const Vector3D& V)
  : x(V.x),
    y(V.y),
    z(V.z)
{
}

Vector3D::Vector3D(Float32 x,
                   Float32 y,
                   Float32 z)
  : x(x),
    y(y),
    z(z)
{
}

Vector3D::~Vector3D()
{
}

Float32
Vector3D::dot(const Vector3D& B) const
{
  return (x*B.x) + (y*B.y) + (z*B.z);
}

Vector3D
Vector3D::cross(const Vector3D& B) const
{
  return Vector3D(y*B.z - z*B.y, z*B.x - x*B.z, x*B.y - y*B.x);
}

Float32
Vector3D::length() const
{
  return sqrt(dot(*this));
}

Float32
Vector3D::lengthSqr() const
{
  return dot(*this);
}

Vector3D
Vector3D::normalize() const
{
  return (*this) * (1 / length());
}

Float32&
Vector3D::operator[](SizeT index)
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
  throw std::out_of_range("Index out of range");
}

Vector3D&
Vector3D::operator=(const Vector3D& A)
{
  x = A.x;
  y = A.y;
  z = A.z;
  return *this;
}

Vector3D
Vector3D::operator+(const Vector3D& A) const {
  return Vector3D(x + A.x, y + A.y, z + A.z);
}

Vector3D&
Vector3D::operator+=(const Vector3D& A) {
  x += A.x;
  y += A.y;
  z += A.z;
  return *this;
}

Vector3D
Vector3D::operator-(const Vector3D& A) const {
  return Vector3D(x - A.x, y - A.y, z - A.z);
}

Vector3D&
Vector3D::operator-=(const Vector3D& A) {
  x -= A.x;
  y -= A.y;
  z -= A.z;
  return *this;
}

Vector3D
Vector3D::operator*(const Vector3D& A) const {
  return Vector3D(x*A.x, y*A.y, z*A.z);
}

Vector3D&
Vector3D::operator*=(const Vector3D& A) {
  x *= A.x;
  y *= A.y;
  z *= A.z;
  return *this;
}

Vector3D
Vector3D::operator*(const Float32 S) const {
  return Vector3D(x*S, y*S, z*S);
}

Vector3D&
Vector3D::operator*=(const Float32 S) {
  x *= S;
  y *= S;
  z *= S;
  return *this;
}

Vector3D
Vector3D::operator/(const Float32 S) const {
  return Vector3D(x*pow(S, -1), y*pow(S, -1), z*pow(S, -1));
}

Vector3D&
Vector3D::operator/=(const Float32 S) {
  x *= pow(S, -1);
  y *= pow(S, -1);
  z *= pow(S, -1);
  return *this;
}

}