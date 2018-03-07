#include <dr_quaternion.h>

namespace driderSDK {

Quaternion::Quaternion() {}

Quaternion::Quaternion(Math::FORCE_INIT k) {
	if (Math::kZero == k) {
		x = 0.f;
		y = 0.f;
		z = 0.f;
		w = 0.f;
	}
	//kIdentity
	else {
		x = 0.f;
		y = 0.f;
		z = 0.f;
		w = 1.f;
	}
}

Quaternion::Quaternion(const Quaternion& Q) : x(Q.x), y(Q.y), z(Q.z), w(Q.w) {}

Quaternion::Quaternion(float _x,
                       float _y,
                       float _z,
                       float _w) : x(_x), y(_y), z(_z), w(_w) {}

Quaternion::Quaternion(const Vector4D& V) {
	x = V.x;
	y = V.y;
	z = V.z;
	w = V.w;
}

Quaternion::~Quaternion() {}

float
Quaternion::measure() {
  return sqrtf(x*x + y*y + z*z + w*w);
}

Quaternion
Quaternion::conjugate() {
  return Quaternion(-x, -y, -z, w);
}

void
Quaternion::normalize() {
	DR_ASSERT(measure() != 0.0f);
  *this *= 1 / measure();
}

const Vector3D
Quaternion::rotation(const Vector3D& V) const {

	Quaternion norm(*this);
	if (norm.measure() != 1.f) {
		norm.normalize();
	}

	Vector3D U(norm.x, norm.y, norm.z);
	Vector3D Result = U * 2.f * U.dot(V) + V *(norm.w*norm.w - U.dot(U)) + 
		U.cross(V) * 2.f * norm.w;
	return Result;
}

void
Quaternion::matrixFromQuaternion(Matrix4x4& MatrixOut) {
	Quaternion Normalized(*this);
	if (Normalized.measure() != 1.f) {
		Normalized.normalize();
	}
  MatrixOut[0][0] = 1.f - (2.f*Normalized.y*Normalized.y) - (2.f*Normalized.z*Normalized.z);
  MatrixOut[1][0] = (2.f*Normalized.x*Normalized.y) - (2.f*Normalized.z*Normalized.w);
  MatrixOut[2][0] = (2.f*Normalized.x*Normalized.z) + (2.f*Normalized.w*Normalized.y);
  MatrixOut[0][1] = (2.f*Normalized.x*Normalized.y) + (2.f*Normalized.z*Normalized.w);
  MatrixOut[1][1] = 1.f - (2.f*Normalized.x*Normalized.x) - (2.f*Normalized.z*Normalized.z);
  MatrixOut[2][1] = (2.f*Normalized.y*Normalized.z) - (2.f*Normalized.x*Normalized.w);
  MatrixOut[0][2] = (2.f*Normalized.x*Normalized.z) - (2.f*Normalized.y*Normalized.w);
  MatrixOut[1][2] = (2.f*Normalized.y*Normalized.z) + (2.f*Normalized.x*Normalized.w);
  MatrixOut[2][2] = 1.f - (2.f*Normalized.x*Normalized.x) - (2.f*Normalized.y*Normalized.y);
  MatrixOut[3][3] = 1.f;
  MatrixOut[3][0] = MatrixOut[1][3] = MatrixOut[2][3] = 0.f;
  MatrixOut[0][3] = MatrixOut[3][1] = MatrixOut[3][2] = 0.f;
}

void
Quaternion::matrixFromQuaternion(Matrix3x3& MatrixOut) {
	Quaternion Normalized(*this);
	if (Normalized.measure() != 1.f) {
		Normalized.normalize();
	}
	MatrixOut[0][0] = 1.f - (2.f*Normalized.y*Normalized.y) - (2.f*Normalized.z*Normalized.z);
	MatrixOut[0][1] = (2.f*Normalized.x*Normalized.y) - (2.f*Normalized.z*Normalized.w);
	MatrixOut[0][2] = (2.f*Normalized.x*Normalized.z) + (2.f*Normalized.y*Normalized.w);
	MatrixOut[1][0] = (2.f*Normalized.x*Normalized.y) + (2.f*Normalized.z*Normalized.w);
	MatrixOut[1][1] = 1.f - (2.f*Normalized.x*Normalized.x) - (2.f*Normalized.z*Normalized.z);
	MatrixOut[1][2] = (2.f*Normalized.y*Normalized.z) - (2.f*Normalized.x*Normalized.w);
	MatrixOut[2][0] = (2.f*Normalized.x*Normalized.z) - (2.f*Normalized.y*Normalized.w);
	MatrixOut[2][1] = (2.f*Normalized.y*Normalized.z) + (2.f*Normalized.x*Normalized.w);
	MatrixOut[2][2] = 1.f - (2.f*Normalized.x*Normalized.x) - (2.f*Normalized.y*Normalized.y);
}

Quaternion Quaternion::slerp(const Quaternion& end, float factor) const {
    float cosom = x * end.x + y * end.y + z * end.z + w * end.w;

    Quaternion endMod = end;
    if ( cosom < 0.0f)
    {
        cosom = -cosom;
        endMod.x = -endMod.x; 
        endMod.y = -endMod.y;
        endMod.z = -endMod.z;
        endMod.w = -endMod.w;
    }

    float sclp, sclq;
    if ((1.0f - cosom) > Math::EPSILON)
    {
        float omega, sinom;
        omega = std::acos( cosom);
        sinom = std::sin( omega);
        sclp  = std::sin( (1.0f - factor) * omega) / sinom;
        sclq  = std::sin( factor * omega) / sinom;
    } 
    else
    {
        sclp = 1.0f - factor;
        sclq = factor;
    }

    Quaternion interpolated;

    interpolated.x = sclp * x + sclq * endMod.x;
    interpolated.y = sclp * y + sclq * endMod.y;
    interpolated.z = sclp * z + sclq * endMod.z;
    interpolated.w = sclp * w + sclq * endMod.w;

    return interpolated;
}

float*
Quaternion::ptr() {
	return &data[0];
}

const float*
Quaternion::ptr() const {
	return &data[0];
}

float&
Quaternion::operator[](const SizeT index) {
	DR_ASSERT(index < 4);
	return data[index];
}

const float&
Quaternion::operator[](const SizeT index) const {
	DR_ASSERT(index < 4);
	return data[index];
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
	*this = (*this) * Q;
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
	DR_ASSERT((Q.x + Q.y + Q.z + Q.w) != 0.0f);
	float div = 1 / (Q.x*Q.x + Q.y*Q.y + Q.z*Q.z + Q.w*Q.w);
	Quaternion R(Q.w*x - Q.x*w - Q.y*z + Q.z*y, 
							 Q.w*y + Q.x*z - Q.y*w - Q.z*x,
							 Q.w*z - Q.x*y + Q.y*x - Q.z*w,
							 Q.w*w + Q.x*x + Q.y*y + Q.z*z);
	R *= div;
	return R;
}

Quaternion&
Quaternion::operator/=(const Quaternion& Q) {
	(*this) = *this / Q;
  return *this;
}

Quaternion&
Quaternion::operator=(const Quaternion& Q) {
	x = Q.x;
	y = Q.y;
	z = Q.z;
	w = Q.w;
	return *this;
}

bool
Quaternion::operator==(const Quaternion& Q) {
	return ((x == Q.x) && (y == Q.y) && (z == Q.z) && (w == Q.w));
}

bool
Quaternion::operator!=(const Quaternion& Q) {
	return !(*this == Q);
}

Quaternion
Quaternion::operator-() const {
	return Quaternion(-x, -y, -z, -w);
}

}