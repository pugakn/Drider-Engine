#pragma once

#include <dr_prerequisites.h>
#include "dr_vector4d.h"
#include "dr_quaternion.h"

namespace driderSDK {

class Matrix3x3;
class Quaternion;

/**
* Matrix with 4x4 elements
*
* Sample usage:
* Matrix4x4 matrixName;
*/
class DR_API_EXPORT Matrix4x4
{
 public:
  /**
  * TEST::operatorEqual
  *
  * Default constructor
  *
  */
  Matrix4x4();

  /**
  * TEST::forceInit
  *
  * Default constructor
  *
  * Values are initialized with 0(kZero) or identity matrix(kIdentity).
  */
  explicit Matrix4x4(Math::FORCE_INIT k);

  /**
  * TEST::Constructor3x3
  *
  * Constructor for matrix3x3
  *
  * Values are initialized with values for matrix3x3
  * vector0, vector1 and vector2 
  * values of w initialize in 0 except vector4.w in 1
  *
  */
  explicit Matrix4x4(const Matrix3x3& M);

  /**
  * TEST::ConstructorQuaternion
  *
  * Constructor for quaternion
  * values of w intitalize in 0 and vector4.w in 1
  *
  */
  explicit Matrix4x4(const Quaternion& Q);

  /**
  * TEST::moveConstructor
  *
  * Move constructor
  *
  */
  Matrix4x4(Matrix4x4&& M) = default;

  /**
  * TEST::constructForMatrix4x4
  *
  * Copy constructor
  *
  */
  Matrix4x4(const Matrix4x4& M);

  /**
  * TEST::constructFor16Floats
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
  * TEST::constructFor4Vectors
  *
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
  * TEST::destructor
  *
  * Destructor
  *
  * Default destructor
  *
  */
  ~Matrix4x4();

  /**
  * TEST::determinant
  *
  * Calculates the determinant of the matrix
  *
  * @return
  *   determinant.
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
  */
  Matrix4x4&
  cofactor();

  /**
  * TEST::inverse
  *
  * Calculates the adjugate of the matrix
  * using transpose for cofactor matrix
  *
  * @return
  *   adjugate.
  */
  Matrix4x4&
  adjugate();

  /**
  * TEST::inverse
  *
  * Calculate inverse of matrix.
  * using det * adjugate
  */
  Matrix4x4&
  inverse();

  /**
  * TEST::inverse
  *
  * Transpose
  *
  * Invert columns whith rows
  * | A1 | A2 | A3 | A4 |T    | A1 | B1 | C1 | D1 |
  * | B1 | B2 | B3 | B4 |     | A2 | B2 | C2 | D2 |
  * | C1 | C2 | C3 | C4 |  =  | A3 | B3 | C3 | D3 |
  * | D1 | D2 | D3 | C4 |     | A4 | B4 | C4 | D4 |
  */
  Matrix4x4&
  transpose();

  /**
  * TEST::identity
  *
  * Identity
  *
  * Tranform matrix to identity
  */
  Matrix4x4&
  identity();

  /**
  * TEST::equals
  *
  * Check's if the other matrix is equal to this matrix with an error
  * range.
  *
  * @param otherMatrix
  *   Matrix to compare with this matrix.
  *
  * @param errorRange
  *   The value of the error range.
  *
  * @return
  *   bool expression of condition.
  */
  bool
  equals(const Matrix4x4& otherMatrix, float errorRange = Math::SMALL_NUMBER) const;

  /**
  * TEST::Translation
  *
  * Translate
  *
  * Change position to vector
  */
  Matrix4x4&
  Translation(const Vector3D &Pos);

  /**
  * TEST::Move
  *
  * Move
  *
  * Add position to vector
  */
  Matrix4x4&
  Move(const Vector3D &Move);

  /**
  * TEST::Scale
  *
  * Scale
  *
  * Change scale of matrix
  */
  Matrix4x4&
  Scale(const Vector3D &Scale);

  /**
  * TEST::Rotation
  *
  * Rotation
  *
  * Aplication rotation z, x, y
  */
  Matrix4x4&
  Rotation(const float Yaw, const float Pitch, const float Roll);

  /**
  * TEST::RotationX
  *
  * Rotation
  *
  * Aplication rotation x
  */
  Matrix4x4&
  RotationX(const float teta);

  /**
  * TEST::RotationY
  *
  * RotationY
  *
  * Aplication rotation Y
  */
  Matrix4x4&
  RotationY(const float teta);

  /**
  * TEST::RotationZ
  *
  * RotationZ
  *
  * Aplication rotation Z
  */
  Matrix4x4&
  RotationZ(const float teta);

  /**
  * TEST:
  */
  Matrix4x4&
  LookAt(const Vector3D &Eye, const Vector3D &At, const Vector3D &Up);

  /**
  * TEST:
  */
 /* Matrix4x4&
  Projection();*/

  /**
  * TEST:
  */
  /*Matrix4x4&
  Orthographic();*/

  /**
  * TEST:
  */
  Matrix4x4&
  Orthogonal(float Width, float Height, float ZNear, float ZFar);

  /**
  * TEST:
  */
  /*Matrix4x4&
  Reflection();*/
  

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
  Vector4D&
  operator[](SizeT index);

  /**
  * TEST::operatorEqual
  */
  Matrix4x4&
  operator=(const Matrix4x4& A);

  /**
  * TEST::operatorEqualMatrix3x3
  * w value not modified or asigned
  */
  Matrix4x4&
  operator=(const Matrix3x3& A);

  /**
  * TEST::operatorAdd
  */
  Matrix4x4
  operator+(const Matrix4x4& A) const;

  /**
  * TEST::operatorAddEqual
  */
  Matrix4x4&
  operator+=(const Matrix4x4& A);

  /**
  * TEST::operatorMinus
  */
  Matrix4x4
  operator-(const Matrix4x4& A) const;

  /**
  * TEST::operatorMinusEqual
  */
  Matrix4x4&
  operator-=(const Matrix4x4& A);

  /**
  * TEST::operatorMultiplication
  */
  Matrix4x4
  operator*(const Matrix4x4& A) const;

  /**
  * TEST::operatorMultiplicationEqual
  */
  Matrix4x4&
  operator*=(const Matrix4x4& A);

  /**
  * TEST::operatorMultiplicationForVector4
  */
  Vector4D
  operator*(const Vector4D& S) const;

  /**
  * TEST::operatorMultiplicationFloat
  */
  Matrix4x4
  operator*(const float S) const;

  /**
  * TEST::operatorMultiplicationEqualFloat
  */
  Matrix4x4&
  operator*=(const float S);

   /**
  * TEST::operatorDivide
  */
  Matrix4x4
  operator/(const Matrix4x4& M) const;

  /**
  * TEST::operatorDivideEqual
  */
  Matrix4x4&
  operator/=(const Matrix4x4& M);

  /**
  * TEST::operatorEqualEqual
  */
  bool
  operator==(const Matrix4x4& M);

  /**
  * TEST::operatorNotEqual
  */
  bool
  operator!=(const Matrix4x4& M);

  union
  {
    struct
    {
      Vector4D vector0, vector1, vector2, vector3;
    };
    Vector4D data[4];
  };

  static Matrix4x4 identityMat4x4;
  static Matrix4x4 zerosMat4x4;
};

}