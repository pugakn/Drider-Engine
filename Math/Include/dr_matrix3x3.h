#pragma once

#include <cassert>
#include <dr_prerequisites.h>
#include "dr_vector3d.h"

namespace driderSDK {

 class Matrix4x4;
 /**
 * Matrix with 3x3 elements
 *
 * Sample usage:
 * Matrix3x3 matrixName;
 */
class DR_API_EXPORT Matrix3x3                                                                 
{
 public:
  /**
  * TEST::operatorEqual
  *
  * Default constructor
  *
  */
  Matrix3x3();

  /**
  * TEST::forceInit
  *
  * Default constructor
  *
  * Values are initialized with 0(kZero) or identity matrix(kIdentity).
  *
  */
  explicit Matrix3x3(Math::FORCE_INIT k);

  /**
  * TEST::moveConstructor
  *
  * Move constructor
  *
  */
  Matrix3x3(Matrix3x3&& M) = default;

  /**
  * TEST::constructForMatrix3x3
  *
  * Copy constructor
  *
  */
  Matrix3x3(const Matrix3x3& M);

  /**
  * TEST::constructFor9Floats
  *
  * Initialize constructor with values.
  *
  * @param v0x, v0y, v0z
  *  The x, y, z values of the vector, first raw
  *
  * @param v1x, v1y, v1z
  *  The x, y, z values of the vector, second raw
  *
  * @param v2x, v2y, v2z
  *  The x, y, z values of the vector, third raw
  *
  */
  Matrix3x3(float v0x, float v0y, float v0z,
            float v1x, float v1y, float v1z,
            float v2x, float v2y, float v2z);

  /**
  * TEST::constructFor3Vectors
  *
  * Initialize constructor with values.
  *
  * @param vector0
  *  The vector0(x, y, z) values of the vector, first row
  *
  * @param vector10-vector12
  *  The vector1(x, y, z) values of the vector, second row
  *
  * @param vector20-vector22
  *  The vector2(x, y, z) values of the vector, third row
  *
  */
  Matrix3x3(Vector3D vector0, Vector3D vector1, Vector3D vector2);

  /**
  * TEST::destructor
  * Default destructor
  *
  */
  ~Matrix3x3();

  /**
  * TEST::determinant
  *
  * Calculates the determinant of the matrix
  *
  * @return
  *   determinant.
  *
  */
  float
  determinant() const;

  /**
  * TEST::inverse
  *
  * Calculates the cofactor of the matrix
  *
  * @return
  *   cofactor of matrix.
  *
  */
  void
  cofactor();

  /**
  *
  * TEST::inverse
  *
  * Calculates the adjugate of the matrix
  * using transpose for cofactor matrix 
  * 
  * @return
  *   adjugate.
  *
  */
  void
  adjugate();

  /**
  * TEST::inverse
  *
  * Calculate inverse of matrix.
  * using det * adjugate
  *
  */
  void
  inverse();

  /**
  * TEST::inverse
  *
  * Transpose
  *
  * Invert columns whith rows
  * | A1 | A2 | A3 |T    | A1 | B1 | C1 |
  * | B1 | B2 | B3 |  =  | A2 | B2 | C2 |
  * | C1 | C2 | C3 |     | A3 | B3 | C3 |
  *
  */
  void
  transpose();

  /**
  * TEST::identity
  *
  * Identity
  *
  * Tranform matrix to identity
  *
  */
  void
  identity();

  /**
  * TEST::getPointer
  *
  * Gets a pointer to the first element of the matrix.
  *
  * @return
  *	  A pointer to the first element of the matrix.
  */
  float*
  ptr();

  /**
  * TEST::getPointerConst
  *
  * Gets a constant pointer to the first element of the matrix.
  *
  * @return
  *	  A constant pointer to the first element of the matrix.
  */
  const float*
  ptr() const;

  /**
  * TEST::forceInit
  */
  Vector3D&
  operator[](SizeT index);

  /**
  * TEST::operatorEqual
  */
  Matrix3x3&
  operator=(const Matrix3x3& A);

  /**
  * TEST::operatorEqualMatrix4x4
  */
  Matrix3x3&
  operator=(const Matrix4x4& A);

  /**
  * TEST::operatorAdd
  */
  Matrix3x3
  operator+(const Matrix3x3& A) const;

  /**
  * TEST::operatorAddEqual
  */
  Matrix3x3&
  operator+=(const Matrix3x3& A);

  /**
  * TEST::operatorMinus
  */
  Matrix3x3
  operator-(const Matrix3x3& A) const;

  /**
  * TEST::operatorMinusEqual
  */
  Matrix3x3&
  operator-=(const Matrix3x3& A);

  /**
  * TEST::operatorMultiplication
  */
  Matrix3x3
  operator*(const Matrix3x3& A) const;

  /**
  * TEST::operatorMultiplicationEqual
  */
  Matrix3x3&
  operator*=(const Matrix3x3& A);

  /**
  * TEST::operatorMultiplicationForVector3
  */
  Vector3D
  operator*(const Vector3D& S) const;

  /**
  * TEST::operatorMultiplicationFloat
  */
  Matrix3x3
  operator*(const float S) const;

  /**
  * TEST::operatorMultiplicationEqualFloat
  */
  Matrix3x3&
  operator*=(const float S);

  /**
  * TEST::operatorEqualEqual
  */
  bool
  operator==(const Matrix3x3& M);

  /**
  * TEST::operatorNotEqual
  */
  bool
  operator!=(const Matrix3x3& M);

  union
  {
    struct
    {
      Vector3D vector0, vector1, vector2;
    };
    Vector3D data[3];
  };

 protected:
};

}