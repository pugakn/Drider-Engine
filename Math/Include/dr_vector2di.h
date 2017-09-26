#pragma once

#include <cassert>
#include <dr_prerequisites.h>
#include "dr_math.h"

namespace driderSDK {

class DR_API_EXPORT Vector2DI
{
 public:
   enum FORCE_INIT
   {
     kZero,
     kIdentity
   };
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
  Vector2DI(FORCE_INIT k);

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
  * @param x
  *  The x value of the vector
  *
  * @param y
  *  The y value of the vector
  */
  Vector2DI(Int32 x, Int32 y);

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
  * Get this vector normalized.
  *
  * @return
  *   This vector normalized.
  */
  Vector2DI
  normalize() const;

  /**
  * Computes the distance between two vectors.
  *
  * @param S
  *   Vector to calculate the distance
  *
  * @return
  *   Distance
  */
  float
  distance(const Vector2DI& S) const;

  /**
  * Computes the squared distance between two vectors.
  *
  * @param S
  *   Vector to calculate the distance
  *
  * @return
  *   Distance
  */
  float
  distanceSqr(const Vector2DI& S) const;

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
  operator[](SizeT index);

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
  operator[](SizeT index) const;

  //Uncommented
  Vector2DI&
  operator=(const Vector2DI& A);

  //Uncommented
  Vector2DI
  operator+(const Vector2DI& A) const;

  //Uncommented
  Vector2DI&
  operator+=(const Vector2DI& A);

  //Uncommented
  Vector2DI
  operator-(const Vector2DI& A) const;

  //Uncommented
  Vector2DI&
  operator-=(const Vector2DI& A);

  //Uncommented
  Vector2DI
  operator*(const Vector2DI& A) const;

  //Uncommented
  Vector2DI&
  operator*=(const Vector2DI& A);

  //Uncommented
  Vector2DI
  operator*(const float S) const;

  //Uncommented
  Vector2DI&
  operator*=(const float S);

  //Uncommented
  Vector2DI
  operator/(const float S) const;

  //Uncommented
  Vector2DI&
  operator/=(const float S);

  //Uncommented
  bool
  operator==(const Vector2DI& S);

  //Uncommented
  bool
  operator!=(const Vector2DI& S);

  //Uncommented
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