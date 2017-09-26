#include <dr_quaternion.h>

namespace driderSDK {

namespace math {

Quaternion::Quaternion()
  : x(0.0f),
	y(0.0f),
	z(0.0f),
	w(1.0f) {

}

Quaternion::Quaternion(const Quaternion & Q)
  : x(Q.x),
	y(Q.y),
	z(Q.z),
	w(Q.w) {
}

Quaternion::Quaternion(float x,
 					   float y,
					   float z,
					   float w)
  : x(x),
	y(y),
	z(z),
	w(w) {
}

Quaternion::~Quaternion() {

}

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
  return (*this)*(1 / this->measure());
}

Quaternion
Quaternion::rotation(float t, const Quaternion & A) {
  Quaternion Axis = A - *this;
  Axis.normalize();
  Axis *= sin(t / 2);
  Axis.w = cos(t / 2);
  return Axis;
}

void
Quaternion::matrixFromQuaternion() {
  // TODO implement matrixFromQuaternion function
}

Quaternion
Quaternion::operator+(const Quaternion & Q) const {
  return Quaternion(x + Q.x, y + Q.y, z + Q.z, w + Q.w);
}

Quaternion&
Quaternion::operator+=(const Quaternion & Q) {
  x += Q.x;
  y += Q.y;
  z += Q.z;
  w += Q.w;
  return *this;
}

Quaternion
Quaternion::operator-(const Quaternion & Q) const {
  return Quaternion(x - Q.x, y - Q.y, z - Q.z, w - Q.w);
}

Quaternion&
Quaternion::operator-=(const Quaternion & Q) {
  x -= Q.x;
  y -= Q.y;
  z -= Q.z;
  w -= Q.w;
  return *this;
}

Quaternion
Quaternion::operator*(const Quaternion & Q) const {
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
Quaternion::operator*=(const Quaternion & Q) {
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
Quaternion::operator/(const Quaternion & Q) const {
  float div = 1 / (Q.x*Q.x + Q.y*Q.y + Q.z*Q.z + Q.w*Q.w);
  Quaternion R(-Q.x, -Q.y, -Q.z, Q.w);
  R *= div;
  return Quaternion((*this)*R);
}

Quaternion&
Quaternion::operator/=(const Quaternion & Q) {
  float div = 1 / (Q.x*Q.x + Q.y*Q.y + Q.z*Q.z + Q.w*Q.w);
  Quaternion R(-Q.x, -Q.y, -Q.z, Q.w);
  R *= div;
  (*this) *= (R * div);
  return *this;
}

}
}
