#include "dr_matrix3x3.h"
#include "dr_matrix4x4.h"

namespace driderSDK
{

Matrix3x3::Matrix3x3() {}

Matrix3x3::Matrix3x3(Math::FORCE_INIT k) {
  if (Math::FORCE_INIT::kZero == k) {
    vector0.x = 0;
    vector0.y = 0;
    vector0.z = 0; 
    
    vector1.x = 0;
    vector1.y = 0;
    vector1.z = 0;

    vector2.x = 0;
    vector2.y = 0;
    vector2.z = 0;    
  }
  //kIdentity
  else {
    vector0.x = 1;
    vector0.y = 0;
    vector0.z = 0;

    vector1.x = 0;
    vector1.y = 1;
    vector1.z = 0;

    vector2.x = 0;
    vector2.y = 0;
    vector2.z = 1;
  }
}

Matrix3x3::Matrix3x3(const Matrix3x3& M)
  : vector0(M.vector0),
    vector1(M.vector1),
    vector2(M.vector2) {}

Matrix3x3::Matrix3x3(float v0x, float v0y, float v0z,
                     float v1x, float v1y, float v1z,
                     float v2x, float v2y, float v2z)
  : vector0(v0x, v0y, v0z),
    vector1(v1x, v1y, v1z),
    vector2(v2x, v2y, v2z) {}

Matrix3x3::Matrix3x3(Vector3D vector0, Vector3D vector1, Vector3D vector2)
  : vector0(vector0),
    vector1(vector1),
    vector2(vector2) {}

Matrix3x3::~Matrix3x3() {}

float 
Matrix3x3::determinant() const {
  return (vector0.x * vector1.y * vector2.z + 
          vector0.y * vector1.z * vector2.x +
          vector0.z * vector1.x * vector2.y) -
         (vector2.x * vector1.y * vector0.z +
          vector2.y * vector1.z * vector0.x +
          vector2.z * vector1.x * vector0.y);
}

void
Matrix3x3::cofactor() {
  Matrix3x3 temp;
  
  temp.vector0.x = vector1.y * vector2.z - vector2.y * vector1.z;
  temp.vector0.y = vector2.x * vector1.z - vector1.x * vector2.z;
  temp.vector0.z = vector1.x * vector2.y - vector2.x * vector1.y;

  temp.vector1.x = vector2.y * vector0.z - vector0.y * vector2.z;
  temp.vector1.y = vector0.x * vector2.z - vector2.x * vector0.z;
  temp.vector1.z = vector2.x * vector0.y - vector0.x * vector2.y;

  temp.vector2.x = vector0.y * vector1.z - vector1.y * vector0.z;
  temp.vector2.y = vector1.x * vector0.z - vector0.x * vector1.z;
  temp.vector2.z = vector0.x * vector1.y - vector1.x * vector0.y;

  *this = temp;
}

void
Matrix3x3::adjugate() {
  cofactor();
  transpose();
}

void 
Matrix3x3::inverse() {
  float deter = determinant();
  DR_ASSERT(deter != 0);
  adjugate();
  *this = *this * (1 / deter);
}

void
Matrix3x3::transpose() {
  Matrix3x3 temp;
  temp.vector0.x = vector0.x;
  temp.vector1.x = vector0.y;
  temp.vector2.x = vector0.z;

  temp.vector0.y = vector1.x;
  temp.vector1.y = vector1.y;
  temp.vector2.y = vector1.z;

  temp.vector0.z = vector2.x;
  temp.vector1.z = vector2.y;
  temp.vector2.z = vector2.z;

  *this = temp;
}

void
Matrix3x3::identity() {
  vector0.x = 1;
  vector0.y = 0;
  vector0.z = 0;

  vector1.x = 0;
  vector1.y = 1;
  vector1.z = 0;

  vector2.x = 0;
  vector2.y = 0;
  vector2.z = 1;
}

float*
Matrix3x3::ptr() {
  return data->ptr();
}

const float*
Matrix3x3::ptr() const {
  return data->ptr();
}

Vector3D&
Matrix3x3::operator[](SizeT index){
  DR_ASSERT(index < 3);
  return data[index];
}

Matrix3x3&
Matrix3x3::operator=(const Matrix3x3& A) {
  vector0 = A.vector0;
  vector1 = A.vector1;
  vector2 = A.vector2;
  return *this;
}

Matrix3x3&
Matrix3x3::operator=(const Matrix4x4& A)
{
  vector0 = A.vector0;
  vector1 = A.vector1;
  vector2 = A.vector2;
  return *this;
}

Matrix3x3
Matrix3x3::operator+(const Matrix3x3& A) const {
  return Matrix3x3(vector0 + A.vector0,
                   vector1 + A.vector1,
                   vector2 + A.vector2);
}

Matrix3x3&
Matrix3x3::operator+=(const Matrix3x3& A) {
  vector0 += A.vector0;
  vector1 += A.vector1;
  vector2 += A.vector2;
  return *this;
}

Matrix3x3
Matrix3x3::operator-(const Matrix3x3& A) const {
  return Matrix3x3(vector0 - A.vector0,
                   vector1 - A.vector1,
                   vector2 - A.vector2);
}

Matrix3x3&
Matrix3x3::operator-=(const Matrix3x3& A) {
  vector0 -= A.vector0;
  vector1 -= A.vector1;
  vector2 -= A.vector2;
  return *this;
}

Matrix3x3
Matrix3x3::operator*(const Matrix3x3& A) const {
  Matrix3x3 temp;

  Vector3D c0(A.vector0.x, A.vector1.x, A.vector2.x);
  Vector3D c1(A.vector0.y, A.vector1.y, A.vector2.y);
  Vector3D c2(A.vector0.z, A.vector1.z, A.vector2.z);
  
  temp.vector0.x = vector0.dot(c0);
  temp.vector0.y = vector0.dot(c1);
  temp.vector0.z = vector0.dot(c2);

  temp.vector1.x = vector1.dot(c0);
  temp.vector1.y = vector1.dot(c1);
  temp.vector1.z = vector1.dot(c2);

  temp.vector2.x = vector2.dot(c0);
  temp.vector2.y = vector2.dot(c1);
  temp.vector2.z = vector2.dot(c2);

  return temp;
}

Matrix3x3&
Matrix3x3::operator*=(const Matrix3x3& A) {
  Matrix3x3 temp;

  Vector3D c0(A.vector0.x, A.vector1.x, A.vector2.x);
  Vector3D c1(A.vector0.y, A.vector1.y, A.vector2.y);
  Vector3D c2(A.vector0.z, A.vector1.z, A.vector2.z);

  temp.vector0.x = vector0.dot(c0);
  temp.vector0.y = vector0.dot(c1);
  temp.vector0.z = vector0.dot(c2);

  temp.vector1.x = vector1.dot(c0);
  temp.vector1.y = vector1.dot(c1);
  temp.vector1.z = vector1.dot(c2);

  temp.vector2.x = vector2.dot(c0);
  temp.vector2.y = vector2.dot(c1);
  temp.vector2.z = vector2.dot(c2);
  
  *this = temp;
  return *this;
}

Vector3D
Matrix3x3::operator*(const Vector3D& S) const {
  Vector3D temp;

  temp.x = vector0.dot(S);
  temp.y = vector1.dot(S);
  temp.z = vector2.dot(S);

  return temp;
}

Matrix3x3
Matrix3x3::operator*(const float S) const {
  Matrix3x3 temp;

  temp.vector0 = vector0 * S;
  temp.vector1 = vector1 * S;
  temp.vector2 = vector2 * S;

  return temp;
}

Matrix3x3&
Matrix3x3::operator*=(const float S) {
  vector0 = vector0 * S;
  vector1 = vector1 * S;
  vector2 = vector2 * S;

  return *this;
}

bool
Matrix3x3::operator==(const Matrix3x3& M) {
  return vector0 == M.vector0 && vector1 == M.vector1 && vector2 == M.vector2;
}

bool
Matrix3x3::operator!=(const Matrix3x3& M) {
  return !(*this == M);
}

}