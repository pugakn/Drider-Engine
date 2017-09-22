#pragma once

#include <stdexcept>
#include <dr_prerequisites.h>
#include "dr_math.h"

namespace driderSDK {

class DR_API_EXPORT Vector4D
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
  * Values are initialized with 0.
  */
  Vector4D();

  /**
  * Default constructor
  *
  * @param k
  *
  * Values are initialized with 0.
  */
  Vector4D(FORCE_INIT k);

  /**
  * Move constructor
  *
  */
  Vector4D(Vector4D&& V) = default;

  /**
  * Copy constructor
  *
  */
  Vector4D(const Vector4D& V);

  /**
  * Initialize constructor with values.
  *
  * @param x
  *  The x value of the vector
  *
  * @param y
  *  The y value of the vector
  *
  * @param z
  *  The z value of the vector
  *
  * @param w
  *  The w value of the vector
  */
  Vector4D(Float32 x, Float32 y, Float32 z, Float32 w);

  /**
  * Default destructor
  *
  */
  ~Vector4D();

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
  Float32
  dot(const Vector4D& B) const;

  /**
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
  * Computes the length of this vector.
  *
  * @return
  *   The length (or "size") of the vector.
  */
  Float32
  length() const;

  /**
  * Computes the squared length of this vector.
  *
  * @return
  *   The length (or "size") of the vector squared.
  */
  Float32
  lengthSqr() const;

  /**
  * Get this vector normalized.
  *
  * @return
  *   This vector normalized.
  */
  Vector4D
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
  Float32
  distance(const Vector4D& S) const;

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
  Float32&
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
  const Float32&
  operator[](SizeT index) const;

  //Uncommented
  Vector4D&
  operator=(const Vector4D& A);

  //Uncommented
  Vector4D
  operator+(const Vector4D& A) const;

  //Uncommented
  Vector4D&
  operator+=(const Vector4D& A);

  //Uncommented
  Vector4D
  operator-(const Vector4D& A) const;

  //Uncommented
  Vector4D&
  operator-=(const Vector4D& A);

  //Uncommented
  Vector4D
  operator*(const Vector4D& A) const;

  //Uncommented
  Vector4D&
  operator*=(const Vector4D& A);

  //Uncommented
  Vector4D
  operator*(const Float32 S) const;

  //Uncommented
  Vector4D&
  operator*=(const Float32 S);

  //Uncommented
  Vector4D
  operator/(const Float32 S) const;

  //Uncommented
  Vector4D&
  operator/=(const Float32 S);

  //Uncommented
  bool
  operator==(const Vector4D& S);

  //Uncommented
  bool
  operator!=(const Vector4D& S);

  Float32 x, y, z, w;
};

}