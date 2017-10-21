#pragma once

#include <dr_prerequisites.h>
#include "dr_math.h"

namespace driderSDK {

class Vector4D;

class DR_API_EXPORT Vector3D
{
 public:
  /**
  * TEST::defaultConstructor
  * Default constructor
  *
  */
  Vector3D();

  /**
  * TEST::forceInit
  * Default constructor
  *
  * @param k
  *
  * Values are initialized with 0.
  */
  explicit Vector3D(Math::FORCE_INIT k);

  /**
  * TEST::moveContructor
- * Move constructor
  *
  */
  Vector3D(Vector3D&& V) = default;

  /**
  * constructForVector3D
  * Copy constructor
  *
  */
  Vector3D(const Vector3D& V);

  /**
  * TEST::constructForVector4D
  * Copy constructor for vec4 to vec3
  *
  */
  explicit Vector3D(const Vector4D& V);

  /**
  * TEST::constructorFor3Floats
  * Initialize constructor with values.
  *
  * @param _x
  *  The x value of the vector
  *
  * @param _y
  *  The y value of the vector
  *
  * @param _z
  *  The z value of the vector
  */
  Vector3D(float _x, float _y, float _z);

  /**
  * TEST::destructor
  * Default destructor
  *
  */
  ~Vector3D();

  /**
  * TEST::dot
  * Computes the dot product between this vector and the vector parameter.
  * This operatios is commutative.
  *
  * @param B
  *  The vector against which the dot product is calculated.
  *
  * @return
  *   The sum of the products of the corresponding entries of the
  *   vectors.
  */
  float
  dot(const Vector3D& B) const;

  /**
  * TEST::cross
  * Computes the cross product between this vector and the vector parameter.
  * This operatios is NOT commutative.
  *
  * @param B
  *  The vector against which the cross product is calculated.
  *  B (vector parameter) is the rigth value of operation
  *  AxB
  *
  * @return
  *   Result vector of the cross product
  */
  Vector3D
  cross(const Vector3D& B) const;

  /**
  * TEST::legth
  * Computes the length of this vector.
  *
  * @return
  *   The length (or "size") of the vector.
  */
  float
  length() const;

  /**
  * TEST::lengthSqr
  * Computes the squared length of this vector.
  *
  * @return
  *   The length (or "size") of the vector squared.
  */
  float
  lengthSqr() const;

  /**
  * TEST::normalize
  * Normalize the vector.
  */
  void
  normalize();

  /**
  * TEST::distance
  * Computes the distance between two vectors.
  *
  * @param scalar
  *   Vector to calculate the distance
  *
  * @return
  *   Distance
  */
  float
  distance(const Vector3D& otherVector) const;

  /**
  * TEST::distanceSqr
  * Computes the squared distance between two vectors.
  *
  * @param scalar
  *   Vector to calculate the distance
  *
  * @return
  *   Distance
  */
  float
  distanceSqr(const Vector3D& otherVector) const;

  /**
  * TEST::equals
  * Check's if the other vector is equal to this vector with an error
  * range.
  *
  * @param otherVector
  *   Vector to compare with this vector.
  *
  * @param errorRange
  *   The value of the error range.
  *
  * @return
  *   bool expression of condition.
  */
  bool
  equals(const Vector3D& otherVector, float errorRange = Math::SMALL_NUMBER) const;

  /**
  * TEST::
  * Computes the squared distance between a point and a segment.
  *
  * @param pointA
  *   Point a of the segment.
  *
  * @param pointB
  *   Point b of the segment.
  *
  * @return
  *   Distance
  */
  float
  sqrDistSegment(const Vector3D& pointA, const Vector3D& pointB) const;

  /**
  * TEST::
  * Gets a pointer to the first element of the matrix.
  *
  * @return
  *	  A pointer to the first element of the matrix.
  */
  float*
  ptr();

  /**
  * TEST::
  * Gets a constant pointer to the first element of the matrix.
  *
  * @return
  *	  A constant pointer to the first element of the matrix.
  */
  const float*
  ptr() const;

  /**
  * TEST::operatorBrackets
  * Gets a reference to the specified element from the vector.
  *
  *	@param index
  *	 The index of the element.
  *
  * @return
  *	  A const reference to the element at the [index] position.
  *
  * @throws out_of_range
  *	  If the index is greater than number of elements in the vector.
  */
  float&
  operator[](const SizeT index);

  /**
  * TEST::operatorBracketsPtr
  * Gets a reference to the specified element from the vector.
  *
  *	@param index
  *	 The index of the element.
  *
  * @return
  *	  A const reference to the element at the [index] position.
  *
  * @throws out_of_range
  *	  If the index is greater than number of elements in the vector.
  */
  const float&
  operator[](const SizeT index) const;

  /**
  * TEST::operatorDot
  * Computes the dot product between this vector and the vector parameter.
  * This operatios is commutative.
  *
  * @param B
  *  The vector against which the dot product is calculated.
  *
  * @return
  *   The sum of the products of the corresponding entries of the
  *   vectors.
  */
  float
  operator|(const Vector3D& B) const;

  /**
  * TEST::operatorCross
  * Computes the cross product between this vector and the vector parameter.
  * This operatios is NOT commutative.
  *
  * @param B
  *  The vector against which the cross product is calculated.
  *  B (vector parameter) is the rigth value of operation
  *  AxB
  *
  * @return
  *   Result vector of the cross product
  */
  Vector3D
  operator^(const Vector3D& B) const;

  /**
  * TEST::operatorEqualVector3
  * Set the x, y, z values of this vector
  * with the x, y, z of the other vector.
  *
  * @param A
  *  The vector with the values to use.
  *
  * @return
  *   A reference to this vector.
  */
  Vector3D&
  operator=(const Vector3D& A);

  /**
  * TEST::operatorEqualVector4
  * Set the x, y, z values of this vector
  * with the x, y, z of the other vector.
  *
  * @param A
  *  The vector with the values to use.
  *
  * @return
  *   A reference to this vector.
  */
  Vector3D&
  operator=(const Vector4D& A);

  /**
  * TEST::operatorAdd
  * Adds elements of the vector with
  * the elements of the other vector.
  *
  * @param A
  *  The vector with the values to use.
  *
  * @return
  *   A vector with the result values.
  */
  Vector3D
  operator+(const Vector3D& A) const;

  /**
  * TEST::operatorAddEqual
  * Adds elements of the vector with
  * the elements of the other vector
  * and stores it in this vector.
  *
  * @param A
  *  The vector with the values to use.
  *
  * @return
  *   A reference to this vector.
  */
  Vector3D&
  operator+=(const Vector3D& A);

  /**
  * TEST::operatorMinus
  * Substracts elements of the vector with
  * the elements of the other vector.
  *
  * @param A
  *  The vector with the values to use.
  *
  * @return
  *   A vector with the result values.
  */
  Vector3D
  operator-(const Vector3D& A) const;

  /**
  * TEST::operatorMinusEqual
  * Substracts elements of the vector with
  * the elements of the other vector and
  * stores it in this vector.
  *
  * @param A
  *  The vector with the values to use.
  *
  * @return
  *   A reference to this vector.
  */
  Vector3D&
  operator-=(const Vector3D& A);

  /**
  * TEST::operatorMultiplication
  * Multiplies elements of the vector with
  * the elements of the other vector.
  *
  * @param A
  *  The vector with the factors to use.
  *
  * @return
  *   A vector with the result values.
  */
  Vector3D
  operator*(const Vector3D& A) const;

  /**
  * TEST::operatorMultiplicationEqual
  * Multiplies elements of the vector with
  * the elements of the other vector, and stores
  * the result in this vector.
  *
  * @param A
  *  The vector with the factors to use.
  *
  * @return
  *   A reference to this vector.
  */
  Vector3D&
  operator*=(const Vector3D& A);

  /**
  * TEST::operatorMultiplicationScalar
  * Multiplies elements of the vector.
  *
  * @param scalar
  *  The factor.
  *
  * @return
  *   A vector with the result values.
  */
  Vector3D
  operator*(const float scalar) const;

  /**
  * TEST::operatorMultiplicationEqualScalar
  * Multiplies elements of the vector and stores it in the vector.
  *
  * @param scalar
  *  The factor.
  *
  * @return
  *   A reference to this vector.
  */
  Vector3D&
  operator*=(const float scalar);

  /**
  * TEST::operatorDivisionScalar
  * Divide elements of the vector.
  *
  * @param scalar
  *  The dividend.
  *
  * @return
  *   A vector with the result values.
  */
  Vector3D
  operator/(const float scalar) const;

  /**
  * TEST::operatorDivisionEqualScalar
  * Divide elements of the vector and stores it in the vector.
  *
  * @param scalar
  *  The dividend.
  *
  * @return
  *   A reference to this vector.
  */
  Vector3D&
  operator/=(const float scalar);

  /**
  * TEST::operatorEqualEqual
  * Checks if the values of the vectors are equal.
  *
  * @return
  *   Flag if vectors are equal.
  */
  bool
  operator==(const Vector3D& otherVector);

  /**
  * TEST::operatorNotEqual
  * Checks if the values of the vectors aren't equal.
  *
  * @return
  *   Flag if vectors aren't equal.
  */
  bool
  operator!=(const Vector3D& otherVector);

  /**
  * TEST::operatorNegate
  * Returns this vector with negated values.
  */
  Vector3D
  operator-() const;

  union
  {
    struct
    {
      float x, y, z;
    };
    float data[3];
  };
};

}