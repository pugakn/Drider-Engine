#pragma once

#include "dr_matrix4x4.h"
#include "dr_matrix3x3.h"
#include "dr_quaternion.h"
#include "dr_plane.h"

namespace driderSDK
{

Matrix4x4::Matrix4x4() {}

Matrix4x4::Matrix4x4(Math::FORCE_INIT k) {
  if (Math::FORCE_INIT::kZero == k) {
    *this = zerosMat4x4;
  }
  //kIdentity
  else {
    *this = identityMat4x4;
  }
}

Matrix4x4::Matrix4x4(const Matrix3x3& M)
  : vector0(M.vector0),
    vector1(M.vector1),
    vector2(M.vector2),
    vector3(Math::FORCE_INIT::kZero) {
  vector0.w = 0.f;
  vector1.w = 0.f;
  vector2.w = 0.f;
  vector3.w = 1.f;
}

Matrix4x4::Matrix4x4(const Quaternion& Q) {
  Matrix3x3 temp(Q);
  Matrix4x4 result(temp);
  (*this) = result;
}

Matrix4x4::Matrix4x4(const Matrix4x4& M)
  : vector0(M.vector0),
    vector1(M.vector1),
    vector2(M.vector2),
    vector3(M.vector3) {}

Matrix4x4::Matrix4x4(float vector0x, float vector0y, float vector0z, float vector0w,
                     float vector1x, float vector1y, float vector1z, float vector1w,
                     float vector2x, float vector2y, float vector2z, float vector2w,
                     float vector3x, float vector3y, float vector3z, float vector3w)
  : vector0(vector0x, vector0y, vector0z, vector0w),
    vector1(vector1x, vector1y, vector1z, vector1w),
    vector2(vector2x, vector2y, vector2z, vector2w),
    vector3(vector3x, vector3y, vector3z, vector3w) {}

Matrix4x4::Matrix4x4(Vector4D vector0,
                     Vector4D vector1,
                     Vector4D vector2,
                     Vector4D vector3)
  : vector0(vector0),
    vector1(vector1),
    vector2(vector2),
    vector3(vector3) {}

Matrix4x4::~Matrix4x4() {}

float
Matrix4x4::determinant() const {
  return vector0.x*vector1.y*vector2.z*vector3.w +
         vector0.x*vector1.z*vector2.w*vector3.y +
         vector0.x*vector1.w*vector2.y*vector3.z +
         vector0.y*vector1.x*vector2.w*vector3.z +
         vector0.y*vector1.z*vector2.x*vector3.w +
         vector0.y*vector1.w*vector2.z*vector3.x +
         vector0.z*vector1.x*vector2.y*vector3.w +
         vector0.z*vector1.y*vector2.w*vector3.x +
         vector0.z*vector1.w*vector2.x*vector3.y +
         vector0.w*vector1.x*vector2.z*vector3.y +
         vector0.w*vector1.y*vector2.x*vector3.z +
         vector0.w*vector1.z*vector2.y*vector3.x -
         vector0.x*vector1.y*vector2.w*vector3.z -
         vector0.x*vector1.z*vector2.y*vector3.w -
         vector0.x*vector1.w*vector2.z*vector3.y -
         vector0.y*vector1.x*vector2.z*vector3.w -
         vector0.y*vector1.z*vector2.w*vector3.x -
         vector0.y*vector1.w*vector2.x*vector3.z -
         vector0.z*vector1.x*vector2.w*vector3.y -
         vector0.z*vector1.y*vector2.x*vector3.w -
         vector0.z*vector1.w*vector2.y*vector3.x -
         vector0.w*vector1.x*vector2.y*vector3.z -
         vector0.w*vector1.y*vector2.z*vector3.x -
         vector0.w*vector1.z*vector2.x*vector3.y;
}

Matrix4x4&
Matrix4x4::cofactor() {
  Matrix4x4 temp;
  
  temp.vector0.x = vector1.y*vector2.z*vector3.w + vector1.z*vector2.w*vector3.y +
                   vector1.w*vector2.y*vector3.z - vector1.y*vector2.w*vector3.z -
                   vector1.z*vector2.y*vector3.w - vector1.w*vector2.z*vector3.y;
  temp.vector1.x = vector0.y*vector2.w*vector3.z + vector0.z*vector2.y*vector3.w +
                   vector0.w*vector2.z*vector3.y - vector0.y*vector2.z*vector3.w -
                   vector0.z*vector2.w*vector3.y - vector0.w*vector2.y*vector3.z;
  temp.vector2.x = vector0.y*vector1.z*vector3.w + vector0.z*vector1.w*vector3.y +
                   vector0.w*vector1.y*vector3.z - vector0.y*vector1.w*vector3.z -
                   vector0.z*vector1.y*vector3.w - vector0.w*vector1.z*vector3.y;
  temp.vector3.x = vector0.y*vector1.w*vector2.z + vector0.z*vector1.y*vector2.w +
                   vector0.w*vector1.z*vector2.y - vector0.y*vector1.z*vector2.w -
                   vector0.z*vector1.w*vector2.y - vector0.w*vector1.y*vector2.z;
  temp.vector0.y = vector1.x*vector2.w*vector3.z + vector1.z*vector2.x*vector3.w +
                   vector1.w*vector2.z*vector3.x - vector1.x*vector2.z*vector3.w -
                   vector1.z*vector2.w*vector3.x - vector1.w*vector2.x*vector3.z;
  temp.vector1.y = vector0.x*vector2.z*vector3.w + vector0.z*vector2.w*vector3.x +
                   vector0.w*vector2.x*vector3.z - vector0.x*vector2.w*vector3.z -
                   vector0.z*vector2.x*vector3.w - vector0.w*vector2.z*vector3.x;
  temp.vector2.y = vector0.x*vector1.w*vector3.z + vector0.z*vector1.x*vector3.w +
                   vector0.w*vector1.z*vector3.x - vector0.x*vector1.z*vector3.w -
                   vector0.z*vector1.w*vector3.x - vector0.w*vector1.x*vector3.z;
  temp.vector3.y = vector0.x*vector1.z*vector2.w + vector0.z*vector1.w*vector2.x +
                   vector0.w*vector1.x*vector2.z - vector0.x*vector1.w*vector2.z -
                   vector0.z*vector1.x*vector2.w - vector0.w*vector1.z*vector2.x;
  temp.vector0.z = vector1.x*vector2.y*vector3.w + vector1.y*vector2.w*vector3.x +
                   vector1.w*vector2.x*vector3.y - vector1.x*vector2.w*vector3.y -
                   vector1.y*vector2.x*vector3.w - vector1.w*vector2.y*vector3.x;
  temp.vector1.z = vector0.x*vector2.w*vector3.y + vector0.y*vector2.x*vector3.w +
                   vector0.w*vector2.y*vector3.x - vector0.x*vector2.y*vector3.w -
                   vector0.y*vector2.w*vector3.x - vector0.w*vector2.x*vector3.y;
  temp.vector2.z = vector0.x*vector1.y*vector3.w + vector0.y*vector1.w*vector3.x +
                   vector0.w*vector1.x*vector3.y - vector0.x*vector1.w*vector3.y -
                   vector0.y*vector1.x*vector3.w - vector0.w*vector1.y*vector3.x;
  temp.vector3.z = vector0.x*vector1.w*vector2.y + vector0.y*vector1.x*vector2.w +
                   vector0.w*vector1.y*vector2.x - vector0.x*vector1.y*vector2.w -
                   vector0.y*vector1.w*vector2.x - vector0.w*vector1.x*vector2.y;
  temp.vector0.w = vector1.x*vector2.z*vector3.y + vector1.y*vector2.x*vector3.z +
                   vector1.z*vector2.y*vector3.x - vector1.x*vector2.y*vector3.z -
                   vector1.y*vector2.z*vector3.x - vector1.z*vector2.x*vector3.y;
  temp.vector1.w = vector0.x*vector2.y*vector3.z + vector0.y*vector2.z*vector3.x +
                   vector0.z*vector2.x*vector3.y - vector0.x*vector2.z*vector3.y -
                   vector0.y*vector2.x*vector3.z - vector0.z*vector2.y*vector3.x;
  temp.vector2.w = vector0.x*vector1.z*vector3.y + vector0.y*vector1.x*vector3.z +
                   vector0.z*vector1.y*vector3.x - vector0.x*vector1.y*vector3.z -
                   vector0.y*vector1.z*vector3.x - vector0.z*vector1.x*vector3.y;
  temp.vector3.w = vector0.x*vector1.y*vector2.z + vector0.y*vector1.z*vector2.x +
                   vector0.z*vector1.x*vector2.y - vector0.x*vector1.z*vector2.y -
                   vector0.y*vector1.x*vector2.z - vector0.z*vector1.y*vector2.x;

  *this = temp;
  return *this;
}

Matrix4x4&
Matrix4x4::adjugate() {
  cofactor();
  transpose();
  return *this;
}

Matrix4x4&
Matrix4x4::inverse() {
  float deter = determinant();
  DR_ASSERT(deter != 0.0f);
  adjugate();
  *this *= Math::pow(deter, -1.f);
  return *this;
}

Matrix4x4&
Matrix4x4::transpose() {
  Matrix4x4 temp;

  temp.vector0.x = vector0.x;
  temp.vector1.x = vector0.y;
  temp.vector2.x = vector0.z;
  temp.vector3.x = vector0.w;

  temp.vector0.y = vector1.x;
  temp.vector1.y = vector1.y;
  temp.vector2.y = vector1.z;
  temp.vector3.y = vector1.w;

  temp.vector0.z = vector2.x;
  temp.vector1.z = vector2.y;
  temp.vector2.z = vector2.z;
  temp.vector3.z = vector2.w;

  temp.vector0.w = vector3.x;
  temp.vector1.w = vector3.y;
  temp.vector2.w = vector3.z;
  temp.vector3.w = vector3.w;
  *this = temp;
  return *this;
}

Matrix4x4&
Matrix4x4::identity() {
  *this = identityMat4x4;
  return *this;
}

bool
Matrix4x4::equals(const Matrix4x4& otherMatrix, float errorRange) const {
  return data[0].equals(otherMatrix.data[0], errorRange) &&
         data[1].equals(otherMatrix.data[1], errorRange) &&
         data[2].equals(otherMatrix.data[2], errorRange) &&
         data[3].equals(otherMatrix.data[3], errorRange);
}

//Vector3D Matrix4x4::eulerAngles()
//{
//  driderSDK::Matrix3x3 tempMatrix(*this);
//  return tempMatrix.eulerAngles();
//}

Matrix4x4&
Matrix4x4::Translation(const Vector3D& Pos) {
  vector3 = Vector4D(Pos, 1.0f);
  return *this;
}

Matrix4x4&
Matrix4x4::Move(const Vector3D& Move) {
  vector3 += Vector4D(Move, 0.0f);
  return *this;
}

Matrix4x4&
Matrix4x4::Scale(const Vector3D &Scale) {
  Matrix4x4 temp(Math::FORCE_INIT::kIdentity);
  temp.vector0.x = Scale.x;
  temp.vector1.y = Scale.y;
  temp.vector2.z = Scale.z;

  *this *= temp;
  return *this;
}

Matrix4x4&
Matrix4x4::Rotation(const float tetax, const float tetay, const float tetaz) {
  RotationY(tetay);
  RotationX(tetax);
  RotationZ(tetaz);
  return *this;
}

Matrix4x4&
Matrix4x4::RotationX(const float teta) {
  float tempCos = Math::cos(teta);
  float tempSin = Math::sin(teta);

  Matrix4x4 temp(Math::FORCE_INIT::kIdentity);
  /*temp[1][1] = tempCos;
  temp[1][2] = -tempSin;
  temp[2][1] = tempSin;
  temp[2][2] = tempCos;*/
  temp[1][1] = tempCos;
  temp[1][2] = tempSin;
  temp[2][1] = -tempSin;
  temp[2][2] = tempCos;

  *this *= temp;
  return *this;
}

Matrix4x4&
Matrix4x4::RotationY(const float teta) {
  float tempCos = Math::cos(teta);
  float tempSin = Math::sin(teta);

  Matrix4x4 temp(Math::FORCE_INIT::kIdentity);
  /*temp[0][0] = tempCos;
  temp[0][2] = tempSin;
  temp[2][0] = -tempSin;
  temp[2][2] = tempCos;*/
  temp[0][0] = tempCos;
  temp[0][2] = -tempSin;
  temp[2][0] = tempSin;
  temp[2][2] = tempCos;

  *this *= temp; 
  return *this;
}

Matrix4x4&
Matrix4x4::RotationZ(const float teta) {
  float tempCos = Math::cos(teta);
  float tempSin = Math::sin(teta);

  Matrix4x4 temp(Math::FORCE_INIT::kIdentity);
  /*temp[0][0] = tempCos;
  temp[0][1] = -tempSin;
  temp[1][0] = tempSin;
  temp[1][1] = tempCos;*/
  temp[0][0] = tempCos;
  temp[0][1] = tempSin;
  temp[1][0] = -tempSin;
  temp[1][1] = tempCos;

  *this *= temp;
  return *this;
}

Matrix4x4&
Matrix4x4::LookAt(const Vector3D& Eye, const Vector3D& At, const Vector3D& Up) {
  Vector3D Axis[3];

  Axis[2] = (At - Eye).normalize();  //zaxis
  Axis[0] = Up.cross(Axis[2]).normalize();  //xaxis
  Axis[1] = Axis[2].cross(Axis[0]); //yaxis

  for (size_t i = 0; i < 3; ++i) {
    (*this)[0][i] = Axis[i].x;
    (*this)[1][i] = Axis[i].y;
    (*this)[2][i] = Axis[i].z;
    (*this)[3][i] = (Axis[i].dot(-Eye));
  }

  (*this)[0][3] = 0.0f;
  (*this)[1][3] = 0.0f;
  (*this)[2][3] = 0.0f;
  (*this)[3][3] = 1.0f;
  return *this;
}

Matrix4x4&
Matrix4x4::Projection(float Width, float Height, float ZNear, float ZFar) {
  *this = zerosMat4x4;

  data[0][0] = 2.0f * ZNear / Width;
  data[1][1] = 2.0f * ZNear / Height;
  data[2][2] = ZFar / (ZFar - ZNear);
  data[2][3] = 1.0f;
  data[3][2] = ZNear * ZFar / (ZNear - ZFar);

  return *this;
}

Matrix4x4&
Matrix4x4::ProjectionFov(float FOV, float Aspect, float ZNear, float ZFar) {
  float SinFov = Math::sin(0.5f * FOV);
  float CosFov = Math::cos(0.5f * FOV);

  float Height = CosFov / SinFov;
  float Width = Height / Aspect;
  float fRange = ZFar / (ZFar - ZNear);

  *this = zerosMat4x4;

  data[0][0] = Width;
  data[1][1] = Height;
  data[2][2] = fRange;
  data[2][3] = 1.0f;
  data[3][2] = -fRange * ZNear;

  return *this;
}

Matrix4x4&
Matrix4x4::Orthogonal(float Width, float Height, float ZNear, float ZFar) {
  (*this) = zerosMat4x4;
  
  (*this)[0][0] = 2.0f / Width;
  (*this)[1][1] = 2.0f / Height;
  (*this)[2][2] = 1.0f / (ZFar - ZNear);
  (*this)[3][2] = -ZNear / (ZNear - ZFar);
  (*this)[3][3] = 1.0f;

  return *this;
}

Matrix4x4&
Matrix4x4::Reflection(Vector3D NormalOfMirror) {
  float xPow2 = NormalOfMirror.x * NormalOfMirror.x;
  float yPow2 = NormalOfMirror.y * NormalOfMirror.y;
  float zPow2 = NormalOfMirror.z * NormalOfMirror.z;
  float xy = -2.f * NormalOfMirror.x * NormalOfMirror.y;
  float xz = -2.f * NormalOfMirror.x * NormalOfMirror.z;
  float yz = -2.f *NormalOfMirror.y * NormalOfMirror.z;

  data[0][0] = -xPow2 + yPow2 + zPow2;
  data[0][1] = xy;
  data[0][2] = xz;
  data[0][3] = 0;

  data[1][0] = xy;
  data[1][1] = +xPow2 - yPow2 + zPow2;
  data[1][2] = yz;
  data[1][3] = 0.f;

  data[2][0] = xz;
  data[2][1] = yz;
  data[2][2] = xPow2 + yPow2 - zPow2;
  data[2][3] = 0.f;

  data[3][0] = 0.f;
  data[3][1] = 0.f;
  data[3][2] = 0.f;
  data[3][3] = 1.f;

  return *this;
}

Vector3D
Matrix4x4::eulerAngles() const {

  //YXZ
  Vector3D angles;

  angles.x = Math::aSin( -Math::clamp( data[1][2], -1.f, 1.f ) );

	if ( Math::abs( data[1][2]) < 0.99999f ) {

		angles.y = Math::aTan2( data[0][2], data[2][2] );
		angles.z = Math::aTan2( data[1][0], data[1][1] );

	} else {

		angles.y = Math::aTan2( -data[2][0], data[0][0] );
		angles.z = 0;
  }

  /*angles.x = Math::aSin( Math::clamp( data[2][1], -1.f, 1.f ) );

	if ( Math::abs( data[2][1] ) < 0.99999f ) {

		angles.y = Math::aTan2( -data[2][0], data[2][2] );
		angles.z = Math::aTan2( -data[0][1], data[1][1] );

	}
  else {
		angles.y = 0;
		angles.z = Math::aTan2( data[1][0], data[0][0] );
  }*/

  /*angles.y = Math::aSin( -Math::clamp( data[2][0], - 1.f, 1.f ) );

	if ( Math::abs( data[2][0] ) < 0.99999f ) {

		angles.x = Math::aTan2( data[2][1], data[2][2] );
		angles.z = Math::aTan2( data[1][0], data[0][0] );

	} 
  else {

		angles.x = 0;
		angles.z = Math::aTan2( - data[0][1], data[1][1] );

  }*/

  return angles;
}

Quaternion 
Matrix4x4::toQuaternion() {
  
  float r22 = data[2][2];

  Quaternion q;

  // x^2 + y^2 >= z^2 + w^2
  if (r22 <= 0.0f) {
    float dif10 = data[1][1] - data[0][0];
    float omr22 = 1.0f - r22;
    if (dif10 <= 0.0f)  // x^2 >= y^2
    {
      float fourXSqr = omr22 - dif10;
      float inv4x = 0.5f / Math::sqrt(fourXSqr);
      q[0] = fourXSqr*inv4x;
      q[1] = (data[0][1] + data[1][0]) * inv4x;
      q[2] = (data[0][2] + data[2][0]) * inv4x;
      q[3] = (data[1][2] - data[2][1]) * inv4x;
    }
    else  // y^2 >= x^2
    {
      float fourYSqr = omr22 + dif10;
      float inv4y = 0.5f / sqrt(fourYSqr);
      q[0] = (data[0][1] + data[1][0]) * inv4y;
      q[1] = fourYSqr * inv4y;
      q[2] = (data[1][2] + data[2][1]) * inv4y;
      q[3] = (data[2][0] - data[0][2]) * inv4y;
    }
  }
  else  // z^2 + w^2 >= x^2 + y^2
  {
    float sum10 = data[1][1] + data[0][0];
    float opr22 = 1.f + r22;
    if (sum10 <= 0.0f)  // z^2 >= w^2
    {
      float fourZSqr = opr22 - sum10;
      float inv4z = 0.5f / sqrt(fourZSqr);
      q[0] = (data[0][2] + data[2][0]) * inv4z;
      q[1] = (data[1][2] + data[2][1]) * inv4z;
      q[2] = fourZSqr * inv4z;
      q[3] = (data[0][1] - data[1][0]) * inv4z;
    }
    else  // w^2 >= z^2
    {
      float fourWSqr = opr22 + sum10;
      float inv4w = 0.5f / sqrt(fourWSqr);

      q[0] = (data[1][2] - data[2][1]) * inv4w;
      q[1] = (data[2][0] - data[0][2]) * inv4w;
      q[2] = (data[0][1] - data[1][0]) * inv4w;
      q[3] = fourWSqr * inv4w;
    }
  }

  return q;
}

float*
Matrix4x4::ptr() {
  return data[0].ptr();
}

const float*
Matrix4x4::ptr() const {
  return data[0].ptr();
}

Vector4D&
Matrix4x4::operator[](SizeT index){
  DR_ASSERT(index < 4);
  return data[index];
}

Matrix4x4&
Matrix4x4::operator=(const Matrix4x4& A) {
  vector0 = A.vector0;
  vector1 = A.vector1;
  vector2 = A.vector2;
  vector3 = A.vector3;
  return *this;
}

Matrix4x4&
Matrix4x4::operator=(const Matrix3x3& A) {
  vector0 = A.vector0;
  vector1 = A.vector1;
  vector2 = A.vector2;
  return *this;
}

Matrix4x4 
Matrix4x4::operator+(const Matrix4x4& A) const {
  return Matrix4x4(vector0 + A.vector0,
                   vector1 + A.vector1,
                   vector2 + A.vector2,
                   vector3 + A.vector3);
}

Matrix4x4&
Matrix4x4::operator+=(const Matrix4x4& A) {
  vector0 += A.vector0;
  vector1 += A.vector1;
  vector2 += A.vector2;
  vector3 += A.vector3;
  return *this;
}

Matrix4x4
Matrix4x4::operator-(const Matrix4x4& A) const {
  return Matrix4x4(vector0 - A.vector0,
                   vector1 - A.vector1,
                   vector2 - A.vector2,
                   vector3 - A.vector3);
}

Matrix4x4&
Matrix4x4::operator-=(const Matrix4x4& A) {
  vector0 -= A.vector0;
  vector1 -= A.vector1;
  vector2 -= A.vector2;
  vector3 -= A.vector3;
  return *this;
}

Matrix4x4
Matrix4x4::operator*(const Matrix4x4& A) const {
  Matrix4x4 temp;

  Vector4D c0(A.vector0.x, A.vector1.x, A.vector2.x, A.vector3.x);
  Vector4D c1(A.vector0.y, A.vector1.y, A.vector2.y, A.vector3.y);
  Vector4D c2(A.vector0.z, A.vector1.z, A.vector2.z, A.vector3.z);
  Vector4D c3(A.vector0.w, A.vector1.w, A.vector2.w, A.vector3.w);


  temp.vector0.x = vector0.dot(c0);
  temp.vector0.y = vector0.dot(c1);
  temp.vector0.z = vector0.dot(c2);
  temp.vector0.w = vector0.dot(c3);

  temp.vector1.x = vector1.dot(c0);
  temp.vector1.y = vector1.dot(c1);
  temp.vector1.z = vector1.dot(c2);
  temp.vector1.w = vector1.dot(c3);

  temp.vector2.x = vector2.dot(c0);
  temp.vector2.y = vector2.dot(c1);
  temp.vector2.z = vector2.dot(c2);
  temp.vector2.w = vector2.dot(c3);

  temp.vector3.x = vector3.dot(c0);
  temp.vector3.y = vector3.dot(c1);
  temp.vector3.z = vector3.dot(c2);
  temp.vector3.w = vector3.dot(c3);

  return temp;
}

Matrix4x4&
Matrix4x4::operator*=(const Matrix4x4& A) {
  Matrix4x4 temp;

  Vector4D c0(A.vector0.x, A.vector1.x, A.vector2.x, A.vector3.x);
  Vector4D c1(A.vector0.y, A.vector1.y, A.vector2.y, A.vector3.y);
  Vector4D c2(A.vector0.z, A.vector1.z, A.vector2.z, A.vector3.z);
  Vector4D c3(A.vector0.w, A.vector1.w, A.vector2.w, A.vector3.w);


  temp.vector0.x = vector0.dot(c0);
  temp.vector0.y = vector0.dot(c1);
  temp.vector0.z = vector0.dot(c2);
  temp.vector0.w = vector0.dot(c3);

  temp.vector1.x = vector1.dot(c0);
  temp.vector1.y = vector1.dot(c1);
  temp.vector1.z = vector1.dot(c2);
  temp.vector1.w = vector1.dot(c3);

  temp.vector2.x = vector2.dot(c0);
  temp.vector2.y = vector2.dot(c1);
  temp.vector2.z = vector2.dot(c2);
  temp.vector2.w = vector2.dot(c3);

  temp.vector3.x = vector3.dot(c0);
  temp.vector3.y = vector3.dot(c1);
  temp.vector3.z = vector3.dot(c2);
  temp.vector3.w = vector3.dot(c3);

  *this = temp;
  return *this;
}

Vector4D
Matrix4x4::operator*(const Vector4D& S) const {
  Vector4D temp;

  temp.x = vector0.dot(S);
  temp.y = vector1.dot(S);
  temp.z = vector2.dot(S);
  temp.w = vector3.dot(S);

  return temp;
}

Matrix4x4
Matrix4x4::operator*(const float S) const {
  Matrix4x4 temp;

  temp.vector0 = vector0 * S;
  temp.vector1 = vector1 * S;
  temp.vector2 = vector2 * S;
  temp.vector3 = vector3 * S;

  return temp;
}

Matrix4x4&
Matrix4x4::operator*=(const float S) {
  vector0 *= S;
  vector1 *= S;
  vector2 *= S;
  vector3 *= S;

  return *this;
}

Matrix4x4
Matrix4x4::operator/(const Matrix4x4 & M) const
{
  Matrix4x4 temp(M);
  return *this * temp.inverse();
}

Matrix4x4&
Matrix4x4::operator/=(const Matrix4x4 & M)
{
  Matrix4x4 temp(M);
  *this *= temp.inverse();
  return *this;
}

bool
Matrix4x4::operator==(const Matrix4x4& M)
{
  return vector0 == M.vector0 &&
         vector1 == M.vector1 &&
         vector2 == M.vector2 &&
         vector3 == M.vector3;
}

bool
Matrix4x4::operator!=(const Matrix4x4& M)
{
  return !(*this == M);
}

Matrix4x4
Matrix4x4::identityMat4x4 = Matrix4x4(1.f, 0.f, 0.f, 0.f,
                                      0.f, 1.f, 0.f, 0.f,
                                      0.f, 0.f, 1.f, 0.f,
                                      0.f, 0.f, 0.f, 1.f);
Matrix4x4
Matrix4x4::zerosMat4x4 = Matrix4x4(0.f, 0.f, 0.f, 0.f,
                                   0.f, 0.f, 0.f, 0.f,
                                   0.f, 0.f, 0.f, 0.f,
                                   0.f, 0.f, 0.f, 0.f);
}