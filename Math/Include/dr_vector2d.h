#pragma once

#include "dr_math_prerequisites.h"
#include "dr_math.h"

namespace driderSDK {

/**
* Vector with 2 elements (x, y)
*
* Sample usage:
* Vector2D vectorName;
*/
class DR_MATH_EXPORT Vector2D
{
 public:
  /**
  * TEST::defaultConstructor
  *
  * Default constructor.
  *
  */
  Vector2D();

  /**
  * TEST::forceInit
  *
  * Default constructor.
  * 
  * @param k
  *
  * Values are initialized with 0.
  */
  explicit Vector2D(Math::FORCE_INIT k);

  /**
  * TEST::moveContructor
  *
  * Move constructor.
  *
  */
  Vector2D(Vector2D&& V) = default;

  /**
  * TEST::copyConstructor
  *
  * Copy constructor.
  *
  */
  Vector2D(const Vector2D& V);

  /**
  * TEST::constructorFor2Floats
  *
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
  * TEST::destructor
  *
  * Default destructor.
  *
  */
  ~Vector2D();

  /**
  * TEST::dot
  *
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
  * TEST::cross
  *
  * Computes the cross product between himself.
  *
  * @return
  *   Result vector of the cross product
  */
  Vector2D
  cross() const;

  /**
  * TEST::length
  *
  * Computes the length of this vector.
  *
  * @return
  *   The length (or "size") of the vector.
  */
  float
  length() const;

  /**
  * TEST::lengthSqr
  *
  * Computes the squared length of this vector.
  *
  * @return
  *   The length (or "size") of the vector squared.
  */
  float
  lengthSqr() const;

  /**
  * TEST::normalize
  *
  * Normalize the vector.
  */
  void
  normalize();

  /**
  * TEST::distance
  *
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
  * TEST::distanceSqr
  *
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
  * TEST::equals
  *
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
  * TEST::compare
  * Compares the polar angle of 2 points.
  *
  * @param p1
  *   Actual point.
  * @param p2
  *   Point to compare.
  *
  * @return
  *   -1: If it's counter clockwise.
  *   +1: If it's clockwise.
  */
  Int32
  compare(const Vector2D& p1, const Vector2D& p2);

  /**
  * TEST::orientation
  * Finds the orientation of ordered triplet p.(q, r).
  *
  * @param q
  *   Point.
  *
  * @param r
  *   Point.
  *
  * @return
  *   0: p, q and r are colinear
  *   1: Clockwise
  *   2: Counterclockwise
  */
  Int32
  orientation(const Vector2D& q, const Vector2D& r) const;

  /**
  * TEST::opPtr
  *
  * Gets a pointer to the first element of the matrix.
  *
  * @return
  *	  A pointer to the first element of the matrix.
  */
  float*
  ptr();

  /**
  * TEST::constOpPtr
  *
  * Gets a constant pointer to the first element of the matrix.
  *
  * @return
  *	  A constant pointer to the first element of the matrix.
  */
  const float*
  ptr() const;

  /**
  * TEST::operatorBrackets
  *
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
  *
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
  * TEST::operatorOr
  *
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
  * TEST::operatorEqual
  *
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
  * TEST::operatorAdd
  *
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
  * TEST::operatorAddEqual
  *
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
  * TEST::operatorMinus
  *
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
  * TEST::operatorMinusEqual
  *
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
  * TEST::operatorMultiplication
  *
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
  * TEST::operatorMultiplicationEqual
  *
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
  * TEST::operatorMultiplicationScalar
  *
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
  * TEST::operatorMultiplicationEqualScalar
  *
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
  * TEST::operatorDivisionScalar
  *
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
  * TEST::operatorDivisionEqualScalar
  *
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
  * TEST::operatorEqualEqual
  *
  * Checks if the values of the vectors are equal.
  *
  * @return
  *   Flag if vectors are equal.
  */
  bool
  operator==(const Vector2D& otherVector);

  /**
  * TEST::operatorNotEqual
  *
  * Checks if the values of the vectors aren't equal.
  *
  * @return
  *   Flag if vectors aren't equal.
  */
  bool
  operator!=(const Vector2D& otherVector);

  /**
  * TEST::operatorNegate
  *
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