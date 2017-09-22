#pragma once

#include "dr_vector3d.h"
#include <dr_prerequisites.h>

namespace driderSDK {

class DR_API_EXPORT Matrix3x3                                                                 
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
  Matrix3x3();

  /**
  * Default constructor
  *
  * Values are initialized with 0(kZero) or identity matrix(kIdentity).
  */
  Matrix3x3(FORCE_INIT k);

  /**
  * Move constructor
  *
  */
  Matrix3x3(Matrix3x3&& M) = default;

  /**
  * Copy constructor
  *
  */
  Matrix3x3(const Matrix3x3& M);

  /**
  * Initialize constructor with values.
  *
  *  @param v0x, v0y, v0z
  *  The x, y, z values of the vector, first raw
  *
  * @param v1x, v1y, v1z
  *  The x, y, z values of the vector, second raw
  *
  * @param v2x, v2y, v2z
  *  The x, y, z values of the vector, third raw
  */
  Matrix3x3(Float32 v0x, Float32 v0y, Float32 v0z,
            Float32 v1x, Float32 v1y, Float32 v1z,
            Float32 v2x, Float32 v2y, Float32 v2z);

  /**
  * Initialize constructor with values.
  *
  * @param v0
  *  The vector0(x, y, z) values of the vector, first raw
  *
  * @param v10-v12
  *  The vector1(x, y, z) values of the vector, second raw
  *
  * @param v20-v22
  *  The vector2(x, y, z) values of the vector, third raw
  */
  Matrix3x3(Vector3D vector0, Vector3D vector1, Vector3D vector2);

  /**
  * Default destructor
  *
  */
  ~Matrix3x3();

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
  Matrix3x3 cofactor() const;

  /**
  * Calculates the adjugate of the matrix
  * using transpose for cofactor matrix 
  * 
  * @return
  *   adjugate.
  */
  Matrix3x3 adjugate() const;

  /**
  * Calculate inverse of matrix.
  * using det * adjugate
  */
  Matrix3x3
  inverse() const;

  /**
  * Transpose
  *
  * Invert columns whith rows
  * | A1 | A2 | A3 |T    | A1 | B1 | C1 |
  * | B1 | B2 | B3 |  =  | A2 | B2 | C2 |
  * | C1 | C2 | C3 |     | A3 | B3 | C3 |
  */
  Matrix3x3
  transpose() const;

  /**
  * Identity
  *
  * Tranform matrix to identity
  */
  Matrix3x3
  identity();

   //Uncommented
  Vector3D&
  operator[](SizeT index);

  //Uncommented
  Matrix3x3&
  operator=(const Matrix3x3& A);

  //Uncommented
  Matrix3x3
  operator+(const Matrix3x3& A) const;

  //Uncommented
  Matrix3x3&
  operator+=(const Matrix3x3& A);

  //Uncommented
  Matrix3x3
  operator-(const Matrix3x3& A) const;

  //Uncommented
  Matrix3x3&
  operator-=(const Matrix3x3& A);

  //Uncommented
  Matrix3x3
  operator*(const Matrix3x3& A) const;

  //Uncommented
  Matrix3x3&
  operator*=(const Matrix3x3& A);

  //Uncommented
  Vector3D
  operator*(const Vector3D S) const;

  //Uncommented
  Matrix3x3
  operator*(const Float32 S) const;

  //Uncommented
  Matrix3x3&
  operator*=(const Float32 S);

  Vector3D v0, v1, v2;

 protected:
};

}