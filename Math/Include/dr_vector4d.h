#pragma once

#include <dr_prerequisites.h>
#include "dr_math.h"

namespace driderSDK {

class Vector3D;

class DR_API_EXPORT Vector4D
{
 public:
  /**
  * TEST::defaultConstructor
  * Default constructor.
  *
  */
  Vector4D();

  /**
  * TEST::forceInit
  * Default constructor.
  *
  * @param k
  *
  * Values are initialized with 0.
  */
  explicit Vector4D(Math::FORCE_INIT k);

  /**
  * TEST::moveContructor
  * Move constructor.
  *
  */
  Vector4D(Vector4D&& V) = default;

  /**
  * TEST::copyConstructor4D
  * Copy constructor.
  *
  */
  Vector4D(const Vector4D& V);

  /**
  * TEST::copyConstructor3D
  * Copy constructor for vec3 to vec4.
  *
  */
  Vector4D(const Vector3D& V);

  /**
  * TEST::copyConstructor3DwithW
  * Copy constructor for vec3 to vec4 with w value.
  *
  */
  Vector4D(const Vector3D& V, float _w);

  /**
  * TEST::floatsConstructor
  * Initialize constructor with values.
  *
  * @param _x
  *  The _x value of the vector
  *
  * @param _y
  *  The _y value of the vector
  *
  * @param _z
  *  The _z value of the vector
  *
  * @param _w
  *  The _w value of the vector
  */
  Vector4D(float _x, float _y, float _z, float _w);

  /**
  * TEST::destructor
  * Default destructor
  *
  */
  ~Vector4D();

  /**
  * TEST::dot3
  * Computes the dot product (excluding w) between this vector and the vector parameter.
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
  dot3(const Vector4D& B) const;

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
  dot(const Vector4D& B) const;

  /**
  * TEST::cross
  * Computes the cross product between this vector and the vector parameter.
  * W value is not used, and it's final value will be 0.
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
  Vector4D
  cross(const Vector4D& B) const;

  /**
  * TEST::length3
  * Computes the length of this vector.
  *
  * @return
  *   The length (or "size") of the vector.
  */
  float
  length3() const;

  /**
  * TEST::length
  * Computes the length of this vector.
  *
  * @return
  *   The length (or "size") of the vector.
  */
  float
  length() const;

  /**
  * TEST::lengthSqr3
  * Computes the squared length of this vector.
  *
  * @return
  *   The length (or "size") of the vector squared.
  */
  float
  lengthSqr3() const;

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
  * TEST::normalize3
  * Normalize the vector.
  */
  void
  normalize3();

  /**
  * TEST::normalize
  * Normalize the vector.
  */
  void
  normalize();

  /**
  * TEST::distance3
  * Computes the distance between two vectors.
  *
  * @param scalar
  *   Vector to calculate the distance
  *
  * @return
  *   Distance
  */
  float
  distance3(const Vector4D& otherVector) const;

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
  distance(const Vector4D& otherVector) const;

  /**
  * TEST::distanceSqr3
  * Computes the squared distance between two vectors.
  *
  * @param scalar
  *   Vector to calculate the distance
  *
  * @return
  *   Distance
  */
  float
  distanceSqr3(const Vector4D& otherVector) const;

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
  distanceSqr(const Vector4D& otherVector) const;

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
  equals(const Vector4D& otherVector, float errorRange = Math::SMALL_NUMBER) const;

  /**
  * TEST::ptrOperator
  * Gets a pointer to the first element of the matrix.
  *
  * @return
  *	  A pointer to the first element of the matrix.
  */
  float*
  ptr();

  /**
  * TEST::constPtrOperator
  * Gets a constant pointer to the first element of the matrix.
  *
  * @return
  *	  A constant pointer to the first element of the matrix.
  */
  const float*
  ptr() const;

  /**
  * TEST::indexOperator
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
  * TEST::constIndexOperator
  * Gets a const reference to the specified element from the vector.
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
  * TEST::dotOperator
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
  operator|(const Vector4D& B) const;

  /**
  * TEST::crossOperator
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
  Vector4D
  operator^(const Vector4D& B) const;

  /**
  * TEST::equalvec4Operator
  * Set the x, y, z, w values of this vector
  * with the x, y, z, w of the other vector.
  *
  * @param A
  *  The vector with the values to use.
  *
  * @return
  *   A reference to this vector.
  */
  Vector4D&
  operator=(const Vector4D& A);

  /**
  * TEST::equalvec3Operator
  * Set the x, y, z values of this vector
  * with the x, y, z of the other vector.
  *
  * @param A
  *  The vector with the values to use.
  *
  * @return
  *   A reference to this vector.
  */
  Vector4D&
  operator=(const Vector3D& A);

  /**
  * TEST::addition
  * Adds elements of the vector with
  * the elements of the other vector.
  *
  * @param A
  *  The vector with the values to use.
  *
  * @return
  *   A vector with the result values.
  */
  Vector4D
  operator+(const Vector4D& A) const;

  /**
  * TEST::addEqual
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
  Vector4D&
  operator+=(const Vector4D& A);

  /**
  * TEST::substraction
  * Substracts elements of the vector with
  * the elements of the other vector.
  *
  * @param A
  *  The vector with the values to use.
  *
  * @return
  *   A vector with the result values.
  */
  Vector4D
  operator-(const Vector4D& A) const;

  /**
  * TEST::lessEqual
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
  Vector4D&
  operator-=(const Vector4D& A);

  /**
  * TEST::multiplication
  * Multiplies elements of the vector with
  * the elements of the other vector.
  *
  * @param A
  *  The vector with the factors to use.
  *
  * @return
  *   A vector with the result values.
  */
  Vector4D
  operator*(const Vector4D& A) const;

  /**
  * TEST::mulEqual
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
  Vector4D&
  operator*=(const Vector4D& A);

  /**
  * TEST::multiplicationFloat
  * Multiplies elements of the vector.
  *
  * @param scalar
  *  The factor.
  *
  * @return
  *   A vector with the result values.
  */
  Vector4D
  operator*(const float scalar) const;

  /**
  * TEST::mulEqualFloat
  * Multiplies elements of the vector and stores it in the vector.
  *
  * @param scalar
  *  The factor.
  *
  * @return
  *   A reference to this vector.
  */
  Vector4D&
  operator*=(const float scalar);

  /**
  * TEST::divitionFloat
  * Divide elements of the vector.
  *
  * @param scalar
  *  The dividend.
  *
  * @return
  *   A vector with the result values.
  */
  Vector4D
  operator/(const float scalar) const;

  /**
  * TEST::divEqualFloat
  * Divide elements of the vector and stores it in the vector.
  *
  * @param scalar
  *  The dividend.
  *
  * @return
  *   A reference to this vector.
  */
  Vector4D&
  operator/=(const float scalar);

  /**
  * TEST::equalOperator
  * Checks if the values of the vectors are equal.
  *
  * @return
  *   Flag if vectors are equal.
  */
  bool
  operator==(const Vector4D& otherVector);

  /**
  * TEST::notEqualOperator
  * Checks if the values of the vectors aren't equal.
  *
  * @return
  *   Flag if vectors aren't equal.
  */
  bool
  operator!=(const Vector4D& otherVector);

  /**
  * TEST::unaryLess
  * Returns this vector with negated values.
  */
  Vector4D
  operator-() const;

  union
  {
    struct
    {
      float x, y, z, w;
    };
    float data[4];
  };
};

}