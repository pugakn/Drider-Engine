#include <dr_quaternion.h>

namespace driderSDK {

Quaternion::Quaternion() {

}

Quaternion::Quaternion(const Quaternion& Q)
  : x(Q.x),
	y(Q.y),
	z(Q.z),
	w(Q.w) {
}

Quaternion::Quaternion(Float32 x,
 					   Float32 y,
					   Float32 z,
					   Float32 w)
  : x(x),
	y(y),
	z(z),
	w(w) {
}

Quaternion::~Quaternion() {

}

Float32
Quaternion::measure() {
  return sqrtf(x*x + y*y + z*z + w*w);
}

Quaternion
Quaternion::conjugate() {
  return Quaternion(-x, -y, -z, w);
}

Quaternion
Quaternion::normalize() {
  return (*this)*(1 / this->measure());
}

Quaternion
Quaternion::rotation(Float32 t, const Quaternion& A) {
  Quaternion Axis = A - *this;
  Axis.normalize();
  Axis *= sin(t / 2);
  Axis.w = cos(t / 2);
  return Axis;
}

void
Quaternion::matrixFromQuaternion(Matrix4x4& M) {
  M[0][0] = 1.0f - (2.0f*y*y) - (2.0f*z*z);
  M[0][1] = (2.0f*x*y) - (2.0f*z*w);
  M[0][2] = (2.0f*x*z) + (2.0f*w*y);
  M[1][0] = (2.0f*x*y) + (2.0f*z*w);
  M[1][1] = 1.0f - (2.0f*x*x) - (2.0f*z*z);
  M[1][2] = (2.0f*y*z) - (2.0f*x*w);
  M[2][0] = (2.0f*x*z) - (2.0f*y*w);
  M[2][1] = (2.0f*y*z) + (2.0f*x*w);
  M[2][2] = 1.0f - (2*x*x) - (2.0f*y*y);
  M[3][3] = 1.0f;
  M[0][3] = M[1][3] = M[2][3] = M[3][0] = M[3][1] = M[3][2] = 0.0f;
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
Quaternion::operator*(Float32 s) const {
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
Quaternion::operator*=(Float32 s) {
  x *= s;
  y *= s;
  z *= s;
  w *= s;
  return *this;
}


Quaternion
Quaternion::operator/(const Quaternion& Q) const {
  Float32 div = 1 / (Q.x*Q.x + Q.y*Q.y + Q.z*Q.z + Q.w*Q.w);
  Quaternion R(-Q.x, -Q.y, -Q.z, Q.w);
  R *= div;
  return Quaternion((*this)*R);
}

Quaternion&
Quaternion::operator/=(const Quaternion& Q) {
  Float32 div = 1 / (Q.x*Q.x + Q.y*Q.y + Q.z*Q.z + Q.w*Q.w);
  Quaternion R(-Q.x, -Q.y, -Q.z, Q.w);
  R *= div;
  (*this) *= (R * div);
  return *this;
}

}
