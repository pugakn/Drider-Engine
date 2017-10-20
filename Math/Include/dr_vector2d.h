#pragma once

#include <dr_prerequisites.h>
#include "dr_math.h"

namespace driderSDK {

class DR_API_EXPORT Vector2D
{
 public:
  /**
  * Default constructor.
  *
  */
  Vector2D();

  /**
  * Default constructor.
  * 
  * @param k
  *
  * Values are initialized with 0.
  */
  explicit Vector2D(Math::FORCE_INIT k);

  /**
  * Move constructor.
  *
  */
  Vector2D(Vector2D&& V) = default;

  /**
  * Copy constructor.
  *
  */
  Vector2D(const Vector2D& V);

  /**
  * Initialize constructor with values.
  *
  * @param _x
  *  The x value of the vector.
  *
  * @param _y
  *  The y value of the vector.
  */
  Vector2D(float _x, float _y);

  /**
  * Default destructor.
  *
  */
  ~Vector2D();

  /**
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
  dot(const Vector2D& B) const;

  /**
  * Computes the cross product between himself.
  *
  * @return
  *   Result vector of the cross product
  */
  Vector2D
  cross() const;

  /**
  * Computes the length of this vector.
  *
  * @return
  *   The length (or "size") of the vector.
  */
  float
  length() const;

  /**
  * Computes the squared length of this vector.
  *
  * @return
  *   The length (or "size") of the vector squared.
  */
  float
  lengthSqr() const;

  /**
  * Normalize the vector.
  */
  void
  normalize();

  /**
  * Computes the distance between two vectors.
  *
  * @param otherVector
  *   Vector to calculate the distance.
  *
  * @return
  *   Distance.
  */
  float
  distance(const Vector2D& otherVector) const;

  /**
  * Computes the squared distance between two vectors.
  *
  * @param otherVector
  *   Vector to calculate the distance.
  *
  * @return
  *   Distance.
  */
  float
  distanceSqr(const Vector2D& otherVector) const;

  /**
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
  equals(const Vector2D& otherVector, float errorRange = Math::SMALL_NUMBER) const;

  /**
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

  float&
  operator[](const SizeT index);

  /**
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
  operator|(const Vector2D& B) const;

  /**
  * Set the x, y values of this vector
  * with the x, y of the other vector.
  *
  * @param A
  *  The vector with the values to use.
  *
  * @return
  *   A reference to this vector.
  */
  Vector2D&
  operator=(const Vector2D& A);

  /**
  * Adds elements of the vector with
  * the elements of the other vector.
  *
  * @param A
  *  The vector with the values to use.
  *
  * @return
  *   A vector with the result values.
  */
  Vector2D
  operator+(const Vector2D& A) const;

  /**
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
  Vector2D&
  operator+=(const Vector2D& A);

  /**
  * Substracts elements of the vector with
  * the elements of the other vector.
  *
  * @param A
  *  The vector with the values to use.
  *
  * @return
  *   A vector with the result values.
  */
  Vector2D
  operator-(const Vector2D& A) const;

  /**
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
  Vector2D&
  operator-=(const Vector2D& A);

  /**
  * Multiplies elements of the vector with
  * the elements of the other vector.
  *
  * @param A
  *  The vector with the factors to use.
  *
  * @return
  *   A vector with the result values.
  */
  Vector2D
  operator*(const Vector2D& A) const;

  /**
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
  Vector2D&
  operator*=(const Vector2D& A);

  /**
  * Multiplies elements of the vector.
  *
  * @param scalar
  *  The factor.
  *
  * @return
  *   A vector with the result values.
  */
  Vector2D
  operator*(const float scalar) const;

  /**
  * Multiplies elements of the vector and stores it in the vector.
  *
  * @param scalar
  *  The factor.
  *
  * @return
  *   A reference to this vector.
  */
  Vector2D&
  operator*=(const float scalar);

  /**
  * Divide elements of the vector.
  *
  * @param scalar
  *  The dividend.
  *
  * @return
  *   A vector with the result values.
  */
  Vector2D
  operator/(const float scalar) const;

  /**
  * Divide elements of the vector and stores it in the vector.
  *
  * @param scalar
  *  The dividend.
  *
  * @return
  *   A reference to this vector.
  */
  Vector2D&
  operator/=(const float scalar);

  /**
  * Checks if the values of the vectors are equal.
  *
  * @return
  *   Flag if vectors are equal.
  */
  bool
  operator==(const Vector2D& otherVector);

  /**
  * Checks if the values of the vectors aren't equal.
  *
  * @return
  *   Flag if vectors aren't equal.
  */
  bool
  operator!=(const Vector2D& otherVector);

  /**
  * Returns this vector with negated values.
  */
  Vector2D
  operator-() const;

  union
  {
    struct
    {
      float x, y;
    };
    float data[2];
  };
};

}