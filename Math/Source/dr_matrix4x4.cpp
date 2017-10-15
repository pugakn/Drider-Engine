#include "dr_matrix4x4.h"
#include "dr_matrix3x3.h"

namespace driderSDK
{

Matrix4x4::Matrix4x4() {}

Matrix4x4::Matrix4x4(Math::FORCE_INIT k) {
  if (Math::FORCE_INIT::kZero == k) {
    vector0.x = 0;
    vector0.y = 0;
    vector0.z = 0;
    vector0.w = 0;

    vector1.x = 0;
    vector1.y = 0;
    vector1.z = 0;
    vector1.w = 0;

    vector2.x = 0;
    vector2.y = 0;
    vector2.z = 0;
    vector2.w = 0;

    vector3.x = 0;
    vector3.y = 0;
    vector3.z = 0;
    vector3.w = 0;
  }
  //kIdentity
  else {
    vector0.x = 1;
    vector0.y = 0;
    vector0.z = 0;
    vector0.w = 0;

    vector1.x = 0;
    vector1.y = 1;
    vector1.z = 0;
    vector1.w = 0;

    vector2.x = 0;
    vector2.y = 0;
    vector2.z = 1;
    vector2.w = 0;

    vector3.x = 0;
    vector3.y = 0;
    vector3.z = 0;
    vector3.w = 1;
  }
}

Matrix4x4::Matrix4x4(const Matrix4x4 & M)
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

void
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
}

void
Matrix4x4::adjugate() {
  cofactor();
  transpose();
}

void
Matrix4x4::inverse() {
  float deter = determinant();
  cofactor();
  transpose();
  *this = *this * (1 / deter);
}

void
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
}

void
Matrix4x4::identity() {
  vector0.x = 1;
  vector0.y = 0;
  vector0.z = 0;
  vector0.w = 0;

  vector1.x = 0;
  vector1.y = 1;
  vector1.z = 0;
  vector1.w = 0;

  vector2.x = 0;
  vector2.y = 0;
  vector2.z = 1;
  vector2.w = 0;

  vector3.x = 0;
  vector3.y = 0;
  vector3.z = 0;
  vector3.w = 1;
}

Vector4D&
Matrix4x4::operator[](SizeT index) {
  DR_ASSERT(index >= 0 && index < 4);
  return data[index];
}

Matrix4x4&
Matrix4x4::operator=(const Matrix4x4 & A) {
  vector0 = A.vector0;
  vector1 = A.vector1;
  vector2 = A.vector2;
  vector3 = A.vector3;
  return *this;
}

Matrix4x4&
Matrix4x4::operator=(const Matrix3x3 & A)
{
  vector0 = A.vector0;
  vector1 = A.vector1;
  vector2 = A.vector2;
  return *this;
}

Matrix4x4 
Matrix4x4::operator+(const Matrix4x4 & A) const {
  return Matrix4x4(vector0 + A.vector0,
                   vector1 + A.vector1,
                   vector2 + A.vector2,
                   vector3 + A.vector3);
}

Matrix4x4&
Matrix4x4::operator+=(const Matrix4x4 & A) {
  vector0 += A.vector0;
  vector1 += A.vector1;
  vector2 += A.vector2;
  vector3 += A.vector3;
  return *this;
}

Matrix4x4
Matrix4x4::operator-(const Matrix4x4 & A) const {
  return Matrix4x4(vector0 - A.vector0,
                   vector1 - A.vector1,
                   vector2 - A.vector2,
                   vector3 - A.vector3);
}

Matrix4x4&
Matrix4x4::operator-=(const Matrix4x4 & A) {
  vector0 -= A.vector0;
  vector1 -= A.vector1;
  vector2 -= A.vector2;
  vector3 -= A.vector3;
  return *this;
}

Matrix4x4
Matrix4x4::operator*(const Matrix4x4 & A) const {
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
Matrix4x4::operator*=(const Matrix4x4 & A) {
  Vector4D c0(A.vector0.x, A.vector1.x, A.vector2.x, A.vector3.x);
  Vector4D c1(A.vector0.y, A.vector1.y, A.vector2.y, A.vector3.y);
  Vector4D c2(A.vector0.z, A.vector1.z, A.vector2.z, A.vector3.z);
  Vector4D c3(A.vector0.w, A.vector1.w, A.vector2.w, A.vector3.w);


  vector0.x = vector0.dot(c0);
  vector0.y = vector0.dot(c1);
  vector0.z = vector0.dot(c2);
  vector0.w = vector0.dot(c3);

  vector1.x = vector1.dot(c0);
  vector1.y = vector1.dot(c1);
  vector1.z = vector1.dot(c2);
  vector1.w = vector1.dot(c3);

  vector2.x = vector2.dot(c0);
  vector2.y = vector2.dot(c1);
  vector2.z = vector2.dot(c2);
  vector2.w = vector2.dot(c3);

  vector3.x = vector3.dot(c0);
  vector3.y = vector3.dot(c1);
  vector3.z = vector3.dot(c2);
  vector3.w = vector3.dot(c3);

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
  vector0 = vector0 * S;
  vector1 = vector1 * S;
  vector2 = vector2 * S;
  vector3 = vector3 * S;

  return *this;
}

bool
Matrix4x4::operator==(const Matrix4x4 & M)
{
  return vector0 == M.vector0 &&
         vector1 == M.vector1 &&
         vector2 == M.vector2 &&
         vector3 == M.vector3;
}

bool
Matrix4x4::operator!=(const Matrix4x4 & M)
{
  return !(*this == M);
}

}