#pragma once

#include <dr_prerequisites.h>
#include "dr_math.h"

//TODO
/*
all vectors
InvSqrt();
FastInvSqrt();
*/

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
  * @param x
  *  The x value of the vector.
  *
  * @param y
  *  The y value of the vector.
  */
  Vector2D(float x, float y);

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
  * range of a SMALL_NuMBER.
  *
  * @param otherVector
  *   Vector to compare with this vector.
  *
  * @return
  *   bool expression of condition.
  */
  bool
  equals(const Vector2D& otherVector) const;

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
  equals(const Vector2D& otherVector, float errorRange) const;

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
  float&
  operator[](SizeT index);

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
  operator[](SizeT index) const;

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

  //Uncommented
  Vector2D&
  operator=(const Vector2D& A);

  //Uncommented
  Vector2D
  operator+(const Vector2D& A) const;

  //Uncommented
  Vector2D&
  operator+=(const Vector2D& A);

  //Uncommented
  Vector2D
  operator-(const Vector2D& A) const;

  //Uncommented
  Vector2D&
  operator-=(const Vector2D& A);

  //Uncommented
  Vector2D
  operator*(const Vector2D& A) const;

  //Uncommented
  Vector2D&
  operator*=(const Vector2D& A);

  //Uncommented
  Vector2D
  operator*(const float scalar) const;

  //Uncommented
  Vector2D&
  operator*=(const float scalar);

  //Uncommented
  Vector2D
  operator/(const float scalar) const;

  //Uncommented
  Vector2D&
  operator/=(const float scalar);

  //Uncommented
  bool
  operator==(const Vector2D& otherVector);

  //Uncommented
  bool
  operator!=(const Vector2D& otherVector);

  //Uncommented
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