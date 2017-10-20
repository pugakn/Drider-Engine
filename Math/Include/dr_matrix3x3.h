#pragma once

#include <cassert>
#include <dr_prerequisites.h>
#include "dr_vector3d.h"

namespace driderSDK {

 class Matrix4x4;

class DR_API_EXPORT Matrix3x3                                                                 
{
 public:
  /**
  * Default constructor
  *
  */
  Matrix3x3();

  /**
  * Default constructor
  *
  * Values are initialized with 0(kZero) or identity matrix(kIdentity).
  *
  * \brief TEST::forceInit
  */
  explicit Matrix3x3(Math::FORCE_INIT k);

  /**
  * Move constructor
  *
  */
  Matrix3x3(Matrix3x3&& M) = default;

  /**
  * Copy constructor
  *
  * \brief TEST::constructForMatrix3x3
  */
  Matrix3x3(const Matrix3x3& M);

  /**
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
  * \brief TEST::constructFor9Floats
  */
  Matrix3x3(float v0x, float v0y, float v0z,
            float v1x, float v1y, float v1z,
            float v2x, float v2y, float v2z);

  /**
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
  * Default destructor
  *
  */
  ~Matrix3x3();

  /**
  * Calculates the determinant of the matrix
  *
  * @return
  *   determinant.
  *
  * \brief TEST::determinant
  */
  float
  determinant() const;

  /**
  * Calculates the cofactor of the matrix
  *
  * @return
  *   cofactor of matrix.
  *
  * \brief TEST::inverse
  */
  void
  cofactor();

  /**
  * Calculates the adjugate of the matrix
  * using transpose for cofactor matrix 
  * 
  * @return
  *   adjugate.
  *
  * \brief TEST::inverse
  */
  void
  adjugate();

  /**
  * Calculate inverse of matrix.
  * using det * adjugate
  *
  * \brief TEST::inverse
  */
  void
  inverse();

  /**
  * Transpose
  *
  * Invert columns whith rows
  * | A1 | A2 | A3 |T    | A1 | B1 | C1 |
  * | B1 | B2 | B3 |  =  | A2 | B2 | C2 |
  * | C1 | C2 | C3 |     | A3 | B3 | C3 |
  *
  * \brief TEST::inverse
  */
  void
  transpose();

  /**
  * Identity
  *
  * Tranform matrix to identity
  *
  * \brief TEST::identity
  */
  void
  identity();

  /**
  * Gets a pointer to the first element of the matrix.
  *
  * @return
  *	  A pointer to the first element of the matrix.
  */
  float*
  ptr();

  /**
  * Gets a constant pointer to the first element of the matrix.
  *
  * @return
  *	  A constant pointer to the first element of the matrix.
  */
  const float*
  ptr() const;

  /**
  *
  */
  Vector3D&
  operator[](SizeT index);

  /**
  * \brief TEST::operatorEqual
  */
  Matrix3x3&
  operator=(const Matrix3x3& A);

  /**
  * \brief TEST::operatorEqualMatrix4x4
  */
  Matrix3x3&
  operator=(const Matrix4x4& A);

  /**
  * \brief TEST::operatorAdd
  */
  Matrix3x3
  operator+(const Matrix3x3& A) const;

  /**
  * \brief TEST::operatorAddEqual
  */
  Matrix3x3&
  operator+=(const Matrix3x3& A);

  /**
  * \brief TEST::operatorMinus
  */
  Matrix3x3
  operator-(const Matrix3x3& A) const;

  /**
  * \brief TEST::operatorMinusEqual
  */
  Matrix3x3&
  operator-=(const Matrix3x3& A);

  /**
  * \brief TEST::operatorMultiplication
  */
  Matrix3x3
  operator*(const Matrix3x3& A) const;

  /**
  * \brief TEST::operatorMultiplicationEqual
  */
  Matrix3x3&
  operator*=(const Matrix3x3& A);

  /**
  * \brief TEST::operatorMultiplicationForVector3
  */
  Vector3D
  operator*(const Vector3D& S) const;

  /**
  * \brief TEST::operatorMultiplicationFloat
  */
  Matrix3x3
  operator*(const float S) const;

  /**
  * \brief TEST::operatorMultiplicationEqualFloat
  */
  Matrix3x3&
  operator*=(const float S);

  /**
  * \brief TEST::operatorEqualEqual
  */
  bool
  operator==(const Matrix3x3& M);

  /**
  * \brief TEST::operatorNotEqual
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