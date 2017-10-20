#pragma once

#include <dr_prerequisites.h>
#include "dr_math.h"

namespace driderSDK {

class DR_API_EXPORT Vector2DI
{
 public:
  /**
  * Default constructor
  *
  */
  Vector2DI();

  /**
  * Default constructor
  * 
  * @param k
  *
  * Values are initialized with 0.
  */
  explicit Vector2DI(Math::FORCE_INIT k);

  /**
  * Move constructor
  *
  */
  Vector2DI(Vector2DI&& V) = default;

  /**
  * Copy constructor
  *
  */
  Vector2DI(const Vector2DI& V);

  /**
  * Initialize constructor with values.
  *
  * @param _x
  *  The x value of the vector
  *
  * @param _y
  *  The y value of the vector
  */
  Vector2DI(Int32 _x, Int32 _y);

  /**
  * Default destructor
  *
  */
  ~Vector2DI();

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
  dot(const Vector2DI& B) const;

  /**
  * Computes the cross product between himself.
  *
  * @return
  *   Result vector of the cross product
  */
  Vector2DI
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
  * @param scalar
  *   Vector to calculate the distance
  *
  * @return
  *   Distance
  */
  float
  distance(const Vector2DI& otherVector) const;

  /**
  * Computes the squared distance between two vectors.
  *
  * @param scalar
  *   Vector to calculate the distance
  *
  * @return
  *   Distance
  */
  float
  distanceSqr(const Vector2DI& otherVector) const;

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
  equals(const Vector2DI& otherVector, float errorRange = Math::SMALL_NUMBER) const;

   /**
  * Gets a pointer to the first element of the matrix.
  *
  * @return
  *	  A pointer to the first element of the matrix.
  */
  Int32*
  ptr();

  /**
  * Gets a constant pointer to the first element of the matrix.
  *
  * @return
  *	  A constant pointer to the first element of the matrix.
  */
  const Int32*
  ptr() const;

  /**
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
  Int32&
  operator[](const SizeT index);

  /**
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
  const Int32&
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
  operator|(const Vector2DI& B) const;

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
  Vector2DI&
  operator=(const Vector2DI& A);

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
  Vector2DI
  operator+(const Vector2DI& A) const;

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
  Vector2DI&
  operator+=(const Vector2DI& A);

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
  Vector2DI
  operator-(const Vector2DI& A) const;

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
  Vector2DI&
  operator-=(const Vector2DI& A);

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
  Vector2DI
  operator*(const Vector2DI& A) const;

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
  Vector2DI&
  operator*=(const Vector2DI& A);

  /**
  * Multiplies elements of the vector.
  *
  * @param scalar
  *  The factor.
  *
  * @return
  *   A vector with the result values.
  */
  Vector2DI
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
  Vector2DI&
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
  Vector2DI
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
  Vector2DI&
  operator/=(const float scalar);

  /**
  * Checks if the values of the vectors are equal.
  *
  * @return
  *   Flag if vectors are equal.
  */
  bool
  operator==(const Vector2DI& otherVector);

  /**
  * Checks if the values of the vectors aren't equal.
  *
  * @return
  *   Flag if vectors aren't equal.
  */
  bool
  operator!=(const Vector2DI& otherVector);

  /**
  * Returns this vector with negated values.
  */
  Vector2DI
  operator-() const;

  union
  {
    struct
    {
      Int32 x, y;
    };
    Int32 data[2];
  };
};

}