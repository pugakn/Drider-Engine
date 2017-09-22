#pragma once

#include "dr_vector4d.h"
#include <dr_prerequisites.h>

namespace driderSDK {

class DR_API_EXPORT Matrix4x4
{
 public:
  enum FORCE_INIT {
    kZero,
    kIdentity
  };

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
  Matrix4x4(FORCE_INIT k);

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
  *  The x, y, z, w values of the vector, first raw
  *
  * @param v1x, v1y, v1z, v1w
  *  The x, y, z, w values of the vector, second raw
  *
  * @param v2x, v2y, v2z, v2w
  *  The x, y, z, w values of the vector, third raw
  *
  * @param v3x, v3y, v3z, v3w
  *  The x, y, z, w values of the vector, quarter raw
  *
  */
  Matrix4x4(Float32 v0x, Float32 v0y, Float32 v0z, Float32 v0w,
            Float32 v1x, Float32 v1y, Float32 v1z, Float32 v1w,
            Float32 v2x, Float32 v2y, Float32 v2z, Float32 v2w,
            Float32 v3x, Float32 v3y, Float32 v3z, Float32 v3w);

  /**
  * Initialize constructor with values.
  *
  * @param v0
  *  The vector0(x, y, z, w) values of the vector, first raw
  *
  * @param v1
  *  The vector1(x, y, z, w) values of the vector, second raw
  *
  * @param v3
  *  The vector2(x, y, z, w) values of the vector, third raw
  *
  * @param v4
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
  Float32 determinant() const;

  /**
  * Calculates the cofactor of the matrix
  *
  * @return
  *   cofactor of matrix.
  */
  Matrix4x4 cofactor() const;

  /**
  * Calculates the adjugate of the matrix
  * using transpose for cofactor matrix
  *
  * @return
  *   adjugate.
  */
  Matrix4x4 adjugate() const;

  /**
  * Calculate inverse of matrix.
  * using det * adjugate
  */
  Matrix4x4
  inverse() const;

  /**
  * Transpose
  *
  * Invert columns whith rows
  * | A1 | A2 | A3 | A4 |T    | A1 | B1 | C1 | D1 |
  * | B1 | B2 | B3 | B4 |     | A2 | B2 | C2 | D2 |
  * | C1 | C2 | C3 | C4 |  =  | A3 | B3 | C3 | D3 |
  * | D1 | D2 | D3 | C4 |     | A4 | B4 | C4 | D4 |
  */
  Matrix4x4
  transpose() const;

  /**
  * Identity
  *
  * Tranform matrix to identity
  */
  Matrix4x4
  identity();

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
    operator*(const Float32 S) const;

  //Uncommented
  Matrix4x4&
    operator*=(const Float32 S);

  Vector4D v0, v1, v2, v3;
};

}