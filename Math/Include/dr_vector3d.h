#pragma once

#include <dr_prerequisites.h>
#include "dr_math.h"

namespace driderSDK {

class Vector4D;

class DR_API_EXPORT Vector3D
{
 public:
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
  explicit Vector3D(Math::FORCE_INIT k);

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
  Vector3D(float x, float y, float z);

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
  float
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
  distance(const Vector3D& otherVector) const;

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
  distanceSqr(const Vector3D& otherVector) const;

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
  equals(const Vector3D& otherVector) const;

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
  equals(const Vector3D& otherVector, float errorRange) const;

  /**
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
  operator|(const Vector3D& B) const;

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
  operator^(const Vector3D& B) const;

  //Uncommented
  Vector3D&
  operator=(const Vector3D& A);

  //Uncommented
  Vector3D&
  operator=(const Vector4D& A);

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
  operator*(const float scalar) const;

  //Uncommented
  Vector3D&
  operator*=(const float scalar);

  //Uncommented
  Vector3D
  operator/(const float scalar) const;

  //Uncommented
  Vector3D&
  operator/=(const float scalar);

  //Uncommented
  bool
  operator==(const Vector3D& otherVector);

  //Uncommented
  bool
  operator!=(const Vector3D& otherVector);

  //Uncommented
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