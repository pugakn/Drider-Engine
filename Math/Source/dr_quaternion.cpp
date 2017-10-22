#include <dr_quaternion.h>

namespace driderSDK {

Quaternion::Quaternion() {}

Quaternion::Quaternion(const Quaternion& Q) : x(Q.x), y(Q.y), z(Q.z), w(Q.w) {}

Quaternion::Quaternion(float x,
                       float y,
                       float z,
                       float w) : x(x), y(y), z(z), w(w) {}

Quaternion::~Quaternion() {}

float
Quaternion::measure() {
  return sqrtf(x*x + y*y + z*z + w*w);
}

Quaternion
Quaternion::conjugate() {
  return Quaternion(-x, -y, -z, w);
}

Quaternion
Quaternion::normalize() {
  return (*this)*(1 / measure());
}

Quaternion
Quaternion::rotation(float theta, const Quaternion& A) {
  Quaternion Axis = A - *this;
  Axis.normalize();
  Axis *= sin(theta / 2);
  Axis.w = cos(theta / 2);
  return Axis;
}

void
Quaternion::matrixFromQuaternion(Matrix4x4& Matrix) {
  Matrix[0][0] = 1.f - (2.f*y*y) - (2.f*z*z);
  Matrix[0][1] = (2.f*x*y) - (2.f*z*w);
  Matrix[0][2] = (2.f*x*z) + (2.f*w*y);
  Matrix[1][0] = (2.f*x*y) + (2.f*z*w);
  Matrix[1][1] = 1.f - (2.f*x*x) - (2.f*z*z);
  Matrix[1][2] = (2.f*y*z) - (2.f*x*w);
  Matrix[2][0] = (2.f*x*z) - (2.f*y*w);
  Matrix[2][1] = (2.f*y*z) + (2.f*x*w);
  Matrix[2][2] = 1.f - (2.f*x*x) - (2.f*y*y);
  Matrix[3][3] = 1.f;
  Matrix[0][3] = Matrix[1][3] = Matrix[2][3] = 0.f;
  Matrix[3][0] = Matrix[3][1] = Matrix[3][2] = 0.f;
  return;
}

void
Quaternion::matrixFromQuaternion(Matrix3x3& Matrix) {
	Matrix[0][0] = (2.f*y*y) - (2.f*z*z);
	Matrix[0][1] = (2.f*x*y) - (2.f*z*w);
	Matrix[0][2] = (2.f*x*z) + (2.f*y*w);
	Matrix[1][0] = (2.f*x*y) + (2.f*z*w);
	Matrix[1][1] = 1.f - (2.f*x*x) - (2.f*z*z);
	Matrix[1][2] = (2.f*y*z) - (2.f*x*w);
	Matrix[2][0] = (2.f*x*z) - (2.f*y*w);
	Matrix[2][1] = (2.f*y*z) + (2.f*x*w);
	Matrix[2][2] = 1.f - (2.f*x*x) - (2.f*y*y);
	return;
}

Quaternion
Quaternion::operator+(const Quaternion& Q) const {
  return Quaternion(x + Q.x, y + Q.y, z + Q.z, w + Q.w);
}

Quaternion&
Quaternion::operator+=(const Quaternion& Q) {
  x += Q.x;
  y += Q.y;
  z += Q.z;
  w += Q.w;
  return *this;
}

Quaternion
Quaternion::operator-(const Quaternion& Q) const {
  return Quaternion(x - Q.x, y - Q.y, z - Q.z, w - Q.w);
}

Quaternion&
Quaternion::operator-=(const Quaternion& Q) {
  x -= Q.x;
  y -= Q.y;
  z -= Q.z;
  w -= Q.w;
  return *this;
}

Quaternion
Quaternion::operator*(const Quaternion& Q) const {
  return Quaternion((w*Q.x) + (y*Q.z) - (z*Q.y) + (x*Q.w),
                    (w*Q.y) + (z*Q.x) - (x*Q.z) + (y*Q.w),
                    (w*Q.z) + (x*Q.y) - (y*Q.x) + (z*Q.w),
                    (w*Q.w) - (x*Q.x) - (y*Q.y) - (z*Q.z));
}

Quaternion
Quaternion::operator*(float s) const {
  return Quaternion(x*s, y*s, z*s, w*s);
}

Quaternion&
Quaternion::operator*=(const Quaternion& Q) {
  x = (w*Q.x) + (y*Q.z) - (z*Q.y) + (x*Q.w);
  y = (w*Q.y) + (z*Q.x) - (x*Q.z) + (y*Q.w);
  z = (w*Q.z) + (x*Q.y) - (y*Q.x) + (z*Q.w);
  w = (w*Q.w) - (x*Q.x) - (y*Q.y) - (z*Q.z);
  return *this;
}

Quaternion&
Quaternion::operator*=(float s) {
  x *= s;
  y *= s;
  z *= s;
  w *= s;
  return *this;
}


Quaternion
Quaternion::operator/(const Quaternion& Q) const {
  float div = 1 / (Q.x*Q.x + Q.y*Q.y + Q.z*Q.z + Q.w*Q.w);
  Quaternion R(-Q.x, -Q.y, -Q.z, Q.w);
  R *= div;
  return Quaternion((*this)*R);
}

Quaternion&
Quaternion::operator/=(const Quaternion& Q) {
  float div = 1 / (Q.x*Q.x + Q.y*Q.y + Q.z*Q.z + Q.w*Q.w);
  Quaternion R(-Q.x, -Q.y, -Q.z, Q.w);
  R *= div;
  (*this) *= (R * div);
  return *this;
}

}
