#pragma once

#include "dr_vector4d.h"
#include <dr_prerequisites.h>

namespace driderSDK {

class DR_API_EXPORT Matrix4x4
{
 public:
  /**
  * Default constructor
  *
  */
  Matrix4x4();

  /**
  * Default constructor
  *
  * Values are initialized with 0(kZero) or identity matrix(kIdentity).
  */
  Matrix4x4(math::FORCE_INIT k);

  /**
  * Move constructor
  *
  */
  Matrix4x4(Matrix4x4&& M) = default;

  /**
  * Copy constructor
  *
  */
  Matrix4x4(const Matrix4x4& M);

  /**
  * Initialize constructor with values.
  *
  * @param v0x, v0y, v0z, v0w
  *  The x, y, z, w values of the vector, first row
  *
  * @param v1x, v1y, v1z, v1w
  *  The x, y, z, w values of the vector, second row
  *
  * @param v2x, v2y, v2z, v2w
  *  The x, y, z, w values of the vector, third row
  *
  * @param v3x, v3y, v3z, v3w
  *  The x, y, z, w values of the vector, quarter row
  *
  */
  Matrix4x4(float v0x, float v0y, float v0z, float v0w,
            float v1x, float v1y, float v1z, float v1w,
            float v2x, float v2y, float v2z, float v2w,
            float v3x, float v3y, float v3z, float v3w);

  /**
  * Initialize constructor with values.
  *
  * @param vector0
  *  The vector0(x, y, z, w) values of the vector, first raw
  *
  * @param vector1
  *  The vector1(x, y, z, w) values of the vector, second raw
  *
  * @param vector3
  *  The vector2(x, y, z, w) values of the vector, third raw
  *
  * @param vector4
  *  The vector2(x, y, z, w) values of the vector, quarter raw
  */
  Matrix4x4(Vector4D vector0, Vector4D vector1, Vector4D vector2, Vector4D vector3);

  /**
  * Default destructor
  *
  */
  ~Matrix4x4();

  /**
  * Calculates the determinant of the matrix
  *
  * @return
  *   determinant.
  */
  float determinant() const;

  /**
  * Calculates the cofactor of the matrix
  *
  * @return
  *   cofactor of matrix.
  */
  void cofactor();

  /**
  * Calculates the adjugate of the matrix
  * using transpose for cofactor matrix
  *
  * @return
  *   adjugate.
  */
  void adjugate();

  /**
  * Calculate inverse of matrix.
  * using det * adjugate
  */
  void inverse();

  /**
  * Transpose
  *
  * Invert columns whith rows
  * | A1 | A2 | A3 | A4 |T    | A1 | B1 | C1 | D1 |
  * | B1 | B2 | B3 | B4 |     | A2 | B2 | C2 | D2 |
  * | C1 | C2 | C3 | C4 |  =  | A3 | B3 | C3 | D3 |
  * | D1 | D2 | D3 | C4 |     | A4 | B4 | C4 | D4 |
  */
  void transpose();

  /**
  * Identity
  *
  * Tranform matrix to identity
  */
  void identity();

  //Uncommented
  Vector4D&
  operator[](SizeT index);

  //Uncommented
  Matrix4x4&
  operator=(const Matrix4x4& A);

  //Uncommented
  Matrix4x4
  operator+(const Matrix4x4& A) const;

  //Uncommented
  Matrix4x4&
  operator+=(const Matrix4x4& A);

  //Uncommented
  Matrix4x4
  operator-(const Matrix4x4& A) const;

  //Uncommented
  Matrix4x4&
  operator-=(const Matrix4x4& A);

  //Uncommented
  Matrix4x4
  operator*(const Matrix4x4& A) const;

  //Uncommented
  Matrix4x4&
  operator*=(const Matrix4x4& A);

  //Uncommented
  Vector4D
  operator*(const Vector4D S) const;

  //Uncommented
  Matrix4x4
  operator*(const float S) const;

  //Uncommented
  Matrix4x4&
  operator*=(const float S);

  union
  {
    struct
    {
      Vector4D vector0, vector1, vector2, vector3;
    };
    Vector4D data[4];
  };
};

}