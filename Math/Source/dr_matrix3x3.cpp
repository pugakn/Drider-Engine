#include <dr_matrix3x3.h>

namespace driderSDK {

Matrix3x3::Matrix3x3() {}

Matrix3x3::Matrix3x3(FORCE_INIT k) {
  if (FORCE_INIT::kZero == k) {
    v0.x = 0;
    v0.y = 0;
    v0.z = 0; 
    
    v1.x = 0;
    v1.y = 0;
    v1.z = 0;

    v2.x = 0;
    v2.y = 0;
    v2.z = 0;    
  }
  //kIdentity
  else {
    v0.x = 1;
    v0.y = 0;
    v0.z = 0;

    v1.x = 0;
    v1.y = 1;
    v1.z = 0;

    v2.x = 0;
    v2.y = 0;
    v2.z = 1;
  }
}

Matrix3x3::Matrix3x3(const Matrix3x3& M) : v0(M.v0), v1(M.v1), v2(M.v2) {}

Matrix3x3::Matrix3x3(float v0x, float v0y, float v0z,
                     float v1x, float v1y, float v1z,
                     float v2x, float v2y, float v2z)
  : v0(v0x, v0y, v0z),
    v1(v1x, v1y, v1z),
    v2(v2x, v2y, v2z)
{}

Matrix3x3::Matrix3x3(Vector3D vector0, Vector3D vector1, Vector3D vector2)
  : v0(vector0),
    v1(vector1),
    v2(vector2)
{}

Matrix3x3::~Matrix3x3() {}

float 
Matrix3x3::determinant() const {
  return (v0.x * v1.y * v2.z + v0.y * v1.z * v2.x + v0.z * v1.x * v2.y) -
    (v2.x * v1.y * v0.z + v2.y * v1.z * v0.x + v2.z * v1.x * v0.y);
}

Matrix3x3
Matrix3x3::cofactor() const {
  Matrix3x3 temp;
  
  temp.v0.x = v1.y * v2.z - v2.y * v1.z;
  temp.v0.y = v2.x * v1.z - v1.x * v2.z;
  temp.v0.z = v1.x * v2.y - v2.x * v1.y;

  temp.v1.x = v2.y * v0.z - v0.y * v2.z;
  temp.v1.y = v0.x * v2.z - v2.x * v0.z;
  temp.v1.z = v2.x * v0.y - v0.x * v2.y;

  temp.v2.x = v0.y * v1.z - v1.y * v0.z;
  temp.v2.y = v1.x * v0.z - v0.x * v1.z;
  temp.v2.z = v0.x * v1.y - v1.x * v0.y;

  return temp;
}

Matrix3x3
Matrix3x3::adjugate() const {
  return cofactor().transpose();
}

Matrix3x3 
Matrix3x3::inverse() const {
  return adjugate() * (1 / determinant());
}

Matrix3x3
Matrix3x3::transpose() const {
  Matrix3x3 temp;
  temp.v0.x = v0.x;
  temp.v1.x = v0.y;
  temp.v2.x = v0.z;

  temp.v0.y = v1.x;
  temp.v1.y = v1.y;
  temp.v2.y = v1.z;

  temp.v0.z = v2.x;
  temp.v1.z = v2.y;
  temp.v2.z = v2.z;

  return temp;
}

Matrix3x3
Matrix3x3::identity() {
  v0.x = 1;
  v0.y = 0;
  v0.z = 0;

  v1.x = 0;
  v1.y = 1;
  v1.z = 0;

  v2.x = 0;
  v2.y = 0;
  v2.z = 1;
  
  return *this;
}

Vector3D&
Matrix3x3::operator[](SizeT index) {
  assert(index >= 0 && index < 3);
  return data[index];
}

Matrix3x3&
Matrix3x3::operator=(const Matrix3x3& A) {
  v0 = A.v0;
  v1 = A.v1;
  v2 = A.v2;
  return *this;
}

Matrix3x3
Matrix3x3::operator+(const Matrix3x3& A) const {
  return Matrix3x3(v0 + A.v0, v1 + A.v1, v2 + A.v2);
}

Matrix3x3&
Matrix3x3::operator+=(const Matrix3x3& A) {
  v0 += A.v0;
  v1 += A.v1;
  v2 += A.v2;
  return *this;
}

Matrix3x3
Matrix3x3::operator-(const Matrix3x3& A) const {
  return Matrix3x3(v0 - A.v0, v1 - A.v1, v2 - A.v2);
}

Matrix3x3&
Matrix3x3::operator-=(const Matrix3x3& A) {
  v0 -= A.v0;
  v1 -= A.v1;
  v2 -= A.v2;
  return *this;
}

Matrix3x3
Matrix3x3::operator*(const Matrix3x3& A) const {
  Matrix3x3 temp;

  Vector3D c0(A.v0.x, A.v1.x, A.v2.x);
  Vector3D c1(A.v0.y, A.v1.y, A.v2.y);
  Vector3D c2(A.v0.z, A.v1.z, A.v2.z);
  
  temp.v0.x = v0.dot(c0);
  temp.v0.y = v0.dot(c1);
  temp.v0.z = v0.dot(c2);

  temp.v1.x = v1.dot(c0);
  temp.v1.y = v1.dot(c1);
  temp.v1.z = v1.dot(c2);

  temp.v2.x = v2.dot(c0);
  temp.v2.y = v2.dot(c1);
  temp.v2.z = v2.dot(c2);

  return temp;
}

Matrix3x3&
Matrix3x3::operator*=(const Matrix3x3& A) {
  Vector3D c0(A.v0.x, A.v1.x, A.v2.x);
  Vector3D c1(A.v0.y, A.v1.y, A.v2.y);
  Vector3D c2(A.v0.z, A.v1.z, A.v2.z);

  v0.x = v0.dot(c0);
  v0.y = v0.dot(c1);
  v0.z = v0.dot(c2);

  v1.x = v1.dot(c0);
  v1.y = v1.dot(c1);
  v1.z = v1.dot(c2);

  v2.x = v2.dot(c0);
  v2.y = v2.dot(c1);
  v2.z = v2.dot(c2);

  return *this;
}

Vector3D
Matrix3x3::operator*(const Vector3D S) const {
  Vector3D temp;

  temp.x = v0.dot(S);
  temp.y = v1.dot(S);
  temp.z = v2.dot(S);

  return temp;
}

Matrix3x3
Matrix3x3::operator*(const float S) const {
  Matrix3x3 temp;

  temp.v0 = v0 * S;
  temp.v1 = v1 * S;
  temp.v2 = v2 * S;

  return temp;
}

Matrix3x3&
Matrix3x3::operator*=(const float S) {
  v0 = v0 * S;
  v1 = v1 * S;
  v2 = v2 * S;

  return *this;
}

bool
Matrix3x3::operator==(const Matrix3x3& M) {
  return v0 == M.v0 && v1 == M.v1 && v2 == M.v2;
}

bool
Matrix3x3::operator!=(const Matrix3x3& M) {
  return !(*this == M);
}

}