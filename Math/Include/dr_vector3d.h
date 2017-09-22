#pragma once

#include <stdexcept>
#include <dr_prerequisites.h>
#include "dr_math.h"

namespace driderSDK {

class DR_API_EXPORT Vector3D
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
  Vector3D();

  /**
  * Default constructor
  *
  * @param k
  *
  * Values are initialized with 0.
  */
  Vector3D(FORCE_INIT k);

  /**
  * Move constructor
  *
  */
  Vector3D(Vector3D&& V) = default;

  /**
  * Copy constructor
  *
  */
  Vector3D(const Vector3D& V);

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
  */
  Vector3D(Float32 x, Float32 y, Float32 z);

  /**
  * Default destructor
  *
  */
  ~Vector3D();

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
  dot(const Vector3D& B) const;

  /**
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
  Vector3D
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
  distance(const Vector3D& S) const;

  /**
  * Computes the squared distance between two vectors.
  *
  * @param S
  *   Vector to calculate the distance
  *
  * @return
  *   Distance
  */
  Float32
  distanceSqr(const Vector3D& S) const;

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
  const Float32&
  operator[](SizeT index) const;

  //Uncommented
  Vector3D&
  operator=(const Vector3D& A);

  //Uncommented
  Vector3D
  operator+(const Vector3D& A) const;

  //Uncommented
  Vector3D&
  operator+=(const Vector3D& A);

  //Uncommented
  Vector3D
  operator-(const Vector3D& A) const;

  //Uncommented
  Vector3D&
  operator-=(const Vector3D& A);

  //Uncommented
  Vector3D
  operator*(const Vector3D& A) const;

  //Uncommented
  Vector3D&
  operator*=(const Vector3D& A);

  //Uncommented
  Vector3D
  operator*(const Float32 S) const;

  //Uncommented
  Vector3D&
  operator*=(const Float32 S);

  //Uncommented
  Vector3D
  operator/(const Float32 S) const;

  //Uncommented
  Vector3D&
  operator/=(const Float32 S);

  //Uncommented
  bool
  operator==(const Vector3D& S);

  //Uncommented
  bool
  operator!=(const Vector3D& S);

  Float32 x, y, z;
};

}