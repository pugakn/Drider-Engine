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
  * @param vector0x, vector0y, vector0z, vector0w
  *  The x, y, z, w values of the vector, first row
  *
  * @param vector1x, vector1y, vector1z, vector1w
  *  The x, y, z, w values of the vector, second row
  *
  * @param vector2x, vector2y, vector2z, vector2w
  *  The x, y, z, w values of the vector, third row
  *
  * @param vector3x, vector3y, vector3z, vector3w
  *  The x, y, z, w values of the vector, quarter row
  *
  */
  Matrix4x4(float vector0x, float vector0y, float vector0z, float vector0w,
            float vector1x, float vector1y, float vector1z, float vector1w,
            float vector2x, float vector2y, float vector2z, float vector2w,
            float vector3x, float vector3y, float vector3z, float vector3w);

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