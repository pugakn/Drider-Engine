#include <dr_matrix4x4.h>

namespace driderSDK {

Matrix4x4::Matrix4x4()
{}

Matrix4x4::Matrix4x4(FORCE_INIT k)
{
  if (k == FORCE_INIT::kZero) {
    v0.x = 0;
    v0.y = 0;
    v0.z = 0;
    v0.w = 0;

    v1.x = 0;
    v1.y = 0;
    v1.z = 0;
    v1.w = 0;

    v2.x = 0;
    v2.y = 0;
    v2.z = 0;
    v2.w = 0;

    v3.x = 0;
    v3.y = 0;
    v3.z = 0;
    v3.w = 0;
  }
  //kIdentity
  else {
    v0.x = 1;
    v0.y = 0;
    v0.z = 0;
    v0.w = 0;

    v1.x = 0;
    v1.y = 1;
    v1.z = 0;
    v1.w = 0;

    v2.x = 0;
    v2.y = 0;
    v2.z = 1;
    v2.w = 0;

    v3.x = 0;
    v3.y = 0;
    v3.z = 0;
    v3.w = 1;
  }
}

Matrix4x4::Matrix4x4(const Matrix4x4 & M)
  : v0(M.v0),
    v1(M.v1),
    v2(M.v2),
    v3(M.v3)
{}

Matrix4x4::Matrix4x4(float v0x, float v0y, float v0z, float v0w,
                     float v1x, float v1y, float v1z, float v1w,
                     float v2x, float v2y, float v2z, float v2w,
                     float v3x, float v3y, float v3z, float v3w)
  : v0(v0x, v0y, v0z, v0w),
    v1(v1x, v1y, v1z, v1w),
    v2(v2x, v2y, v2z, v2w),
    v3(v3x, v3y, v3z, v3w)
{}

Matrix4x4::Matrix4x4(Vector4D vector0, Vector4D vector1, Vector4D vector2, Vector4D vector3)
  : v0(vector0),
    v1(vector1),
    v2(vector2),
    v3(vector3)
{}

Matrix4x4::~Matrix4x4()
{
}

float Matrix4x4::determinant() const
{
  return v0.x*v1.y*v2.z*v3.w + v0.x*v1.z*v2.w*v3.y + v0.x*v1.w*v2.y*v3.z +
         v0.y*v1.x*v2.w*v3.z + v0.y*v1.z*v2.x*v3.w + v0.y*v1.w*v2.z*v3.x +
         v0.z*v1.x*v2.y*v3.w + v0.z*v1.y*v2.w*v3.x + v0.z*v1.w*v2.x*v3.y +
         v0.w*v1.x*v2.z*v3.y + v0.w*v1.y*v2.x*v3.z + v0.w*v1.z*v2.y*v3.x -
         v0.x*v1.y*v2.w*v3.z - v0.x*v1.z*v2.y*v3.w - v0.x*v1.w*v2.z*v3.y -
         v0.y*v1.x*v2.z*v3.w - v0.y*v1.z*v2.w*v3.x - v0.y*v1.w*v2.x*v3.z -
         v0.z*v1.x*v2.w*v3.y - v0.z*v1.y*v2.x*v3.w - v0.z*v1.w*v2.y*v3.x -
         v0.w*v1.x*v2.y*v3.z - v0.w*v1.y*v2.z*v3.x - v0.w*v1.z*v2.x*v3.y;
}

Matrix4x4 Matrix4x4::cofactor() const
{
  Matrix4x4 temp;
  
  temp.v0.x = v1.y*v2.z*v3.w + v1.z*v2.w*v3.y + v1.w*v2.y*v3.z -
              v1.y*v2.w*v3.z - v1.z*v2.y*v3.w - v1.w*v2.z*v3.y;
  temp.v1.x = v0.y*v2.w*v3.z + v0.z*v2.y*v3.w + v0.w*v2.z*v3.y -
              v0.y*v2.z*v3.w - v0.z*v2.w*v3.y - v0.w*v2.y*v3.z;
  temp.v2.x = v0.y*v1.z*v3.w + v0.z*v1.w*v3.y + v0.w*v1.y*v3.z -
              v0.y*v1.w*v3.z - v0.z*v1.y*v3.w - v0.w*v1.z*v3.y;
  temp.v3.x = v0.y*v1.w*v2.z + v0.z*v1.y*v2.w + v0.w*v1.z*v2.y -
              v0.y*v1.z*v2.w - v0.z*v1.w*v2.y - v0.w*v1.y*v2.z;
  temp.v0.y = v1.x*v2.w*v3.z + v1.z*v2.x*v3.w + v1.w*v2.z*v3.x -
              v1.x*v2.z*v3.w - v1.z*v2.w*v3.x - v1.w*v2.x*v3.z;
  temp.v1.y = v0.x*v2.z*v3.w + v0.z*v2.w*v3.x + v0.w*v2.x*v3.z -
              v0.x*v2.w*v3.z - v0.z*v2.x*v3.w - v0.w*v2.z*v3.x;
  temp.v2.y = v0.x*v1.w*v3.z + v0.z*v1.x*v3.w + v0.w*v1.z*v3.x -
              v0.x*v1.z*v3.w - v0.z*v1.w*v3.x - v0.w*v1.x*v3.z;
  temp.v3.y = v0.x*v1.z*v2.w + v0.z*v1.w*v2.x + v0.w*v1.x*v2.z -
              v0.x*v1.w*v2.z - v0.z*v1.x*v2.w - v0.w*v1.z*v2.x;
  temp.v0.z = v1.x*v2.y*v3.w + v1.y*v2.w*v3.x + v1.w*v2.x*v3.y -
              v1.x*v2.w*v3.y - v1.y*v2.x*v3.w - v1.w*v2.y*v3.x;
  temp.v1.z = v0.x*v2.w*v3.y + v0.y*v2.x*v3.w + v0.w*v2.y*v3.x -
              v0.x*v2.y*v3.w - v0.y*v2.w*v3.x - v0.w*v2.x*v3.y;
  temp.v2.z = v0.x*v1.y*v3.w + v0.y*v1.w*v3.x + v0.w*v1.x*v3.y -
              v0.x*v1.w*v3.y - v0.y*v1.x*v3.w - v0.w*v1.y*v3.x;
  temp.v3.z = v0.x*v1.w*v2.y + v0.y*v1.x*v2.w + v0.w*v1.y*v2.x -
              v0.x*v1.y*v2.w - v0.y*v1.w*v2.x - v0.w*v1.x*v2.y;
  temp.v0.w = v1.x*v2.z*v3.y + v1.y*v2.x*v3.z + v1.z*v2.y*v3.x -
              v1.x*v2.y*v3.z - v1.y*v2.z*v3.x - v1.z*v2.x*v3.y;
  temp.v1.w = v0.x*v2.y*v3.z + v0.y*v2.z*v3.x + v0.z*v2.x*v3.y -
              v0.x*v2.z*v3.y - v0.y*v2.x*v3.z - v0.z*v2.y*v3.x;
  temp.v2.w = v0.x*v1.z*v3.y + v0.y*v1.x*v3.z + v0.z*v1.y*v3.x -
              v0.x*v1.y*v3.z - v0.y*v1.z*v3.x - v0.z*v1.x*v3.y;
  temp.v3.w = v0.x*v1.y*v2.z + v0.y*v1.z*v2.x + v0.z*v1.x*v2.y -
              v0.x*v1.z*v2.y - v0.y*v1.x*v2.z - v0.z*v1.y*v2.x;

  return temp;
}

Matrix4x4 Matrix4x4::adjugate() const
{
  return cofactor().transpose();
}

Matrix4x4
Matrix4x4::inverse() const
{
  return adjugate() * (1 / determinant());
}

Matrix4x4 Matrix4x4::transpose() const
{
  Matrix4x4 temp;

  temp.v0.x = v0.x;
  temp.v1.x = v0.y;
  temp.v2.x = v0.z;
  temp.v3.x = v0.w;

  temp.v0.y = v1.x;
  temp.v1.y = v1.y;
  temp.v2.y = v1.z;
  temp.v3.y = v1.w;

  temp.v0.z = v2.x;
  temp.v1.z = v2.y;
  temp.v2.z = v2.z;
  temp.v3.z = v2.w;

  temp.v0.w = v3.x;
  temp.v1.w = v3.y;
  temp.v2.w = v3.z;
  temp.v3.w = v3.w;

  return temp;
}

Matrix4x4
Matrix4x4::identity()
{
  v0.x = 1;
  v0.y = 0;
  v0.z = 0;
  v0.w = 0;

  v1.x = 0;
  v1.y = 1;
  v1.z = 0;
  v1.w = 0;

  v2.x = 0;
  v2.y = 0;
  v2.z = 1;
  v2.w = 0;

  v3.x = 0;
  v3.y = 0;
  v3.z = 0;
  v3.w = 1;

  return *this;
}

Vector4D&
Matrix4x4::operator[](SizeT index)
{
  // TODO: insert return statement here
  if (index == 0) {
    return v0;
  }
  else if (index == 1) {
    return v1;
  }
  else if (index == 2) {
    return v2;
  }
  else if (index == 3) {
    return v3;
  }
  throw std::out_of_range("Index out of range");
}

Matrix4x4&
Matrix4x4::operator=(const Matrix4x4 & A)
{
  v0 = A.v0;
  v1 = A.v1;
  v2 = A.v2;
  v3 = A.v3;
  return *this;
}

Matrix4x4 
Matrix4x4::operator+(const Matrix4x4 & A) const
{
  return Matrix4x4(v0 + A.v0, v1 + A.v1, v2 + A.v2, v3 + A.v3);
}

Matrix4x4&
Matrix4x4::operator+=(const Matrix4x4 & A)
{
  v0 += A.v0;
  v1 += A.v1;
  v2 += A.v2;
  v3 += A.v3;
  return *this;
}

Matrix4x4
Matrix4x4::operator-(const Matrix4x4 & A) const
{
  return Matrix4x4(v0 - A.v0, v1 - A.v1, v2 - A.v2, v3 - A.v3);
}

Matrix4x4&
Matrix4x4::operator-=(const Matrix4x4 & A)
{
  v0 -= A.v0;
  v1 -= A.v1;
  v2 -= A.v2;
  v3 -= A.v3;
  return *this;
}

Matrix4x4
Matrix4x4::operator*(const Matrix4x4 & A) const
{
  Matrix4x4 temp;

  Vector4D c0(A.v0.x, A.v1.x, A.v2.x, A.v3.x);
  Vector4D c1(A.v0.y, A.v1.y, A.v2.y, A.v3.y);
  Vector4D c2(A.v0.z, A.v1.z, A.v2.z, A.v3.z);
  Vector4D c3(A.v0.w, A.v1.w, A.v2.w, A.v3.w);


  temp.v0.x = v0.dot(c0);
  temp.v0.y = v0.dot(c1);
  temp.v0.z = v0.dot(c2);
  temp.v0.w = v0.dot(c3);

  temp.v1.x = v1.dot(c0);
  temp.v1.y = v1.dot(c1);
  temp.v1.z = v1.dot(c2);
  temp.v1.w = v1.dot(c3);

  temp.v2.x = v2.dot(c0);
  temp.v2.y = v2.dot(c1);
  temp.v2.z = v2.dot(c2);
  temp.v2.w = v2.dot(c3);

  temp.v3.x = v3.dot(c0);
  temp.v3.y = v3.dot(c1);
  temp.v3.z = v3.dot(c2);
  temp.v3.w = v3.dot(c3);

  return temp;
}

Matrix4x4&
Matrix4x4::operator*=(const Matrix4x4 & A)
{
  Vector4D c0(A.v0.x, A.v1.x, A.v2.x, A.v3.x);
  Vector4D c1(A.v0.y, A.v1.y, A.v2.y, A.v3.y);
  Vector4D c2(A.v0.z, A.v1.z, A.v2.z, A.v3.z);
  Vector4D c3(A.v0.w, A.v1.w, A.v2.w, A.v3.w);


  v0.x = v0.dot(c0);
  v0.y = v0.dot(c1);
  v0.z = v0.dot(c2);
  v0.w = v0.dot(c3);

  v1.x = v1.dot(c0);
  v1.y = v1.dot(c1);
  v1.z = v1.dot(c2);
  v1.w = v1.dot(c3);

  v2.x = v2.dot(c0);
  v2.y = v2.dot(c1);
  v2.z = v2.dot(c2);
  v2.w = v2.dot(c3);

  v3.x = v3.dot(c0);
  v3.y = v3.dot(c1);
  v3.z = v3.dot(c2);
  v3.w = v3.dot(c3);

  return *this;
}

Vector4D
Matrix4x4::operator*(const Vector4D S) const
{
  Vector4D temp;

  temp.x = v0.dot(S);
  temp.y = v1.dot(S);
  temp.z = v2.dot(S);
  temp.w = v3.dot(S);

  return temp;
}

Matrix4x4
Matrix4x4::operator*(const float S) const
{
  Matrix4x4 temp;

  temp.v0 = v0 * S;
  temp.v1 = v1 * S;
  temp.v2 = v2 * S;
  temp.v3 = v3 * S;

  return temp;
}

Matrix4x4&
Matrix4x4::operator*=(const float S)
{
  v0 = v0 * S;
  v1 = v1 * S;
  v2 = v2 * S;
  v3 = v3 * S;

  return *this;
}

}