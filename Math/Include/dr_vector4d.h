#pragma once

#include <dr_prerequisites.h>
#include "dr_math.h"

namespace driderSDK {

class Vector3D;

class DR_API_EXPORT Vector4D
{
 public:
  /**
  * Default constructor.
  *
  */
  Vector4D();

  /**
  * Default constructor.
  *
  * @param k
  *
  * Values are initialized with 0.
  */
  explicit Vector4D(Math::FORCE_INIT k);

  /**
  * Move constructor.
  *
  */
  Vector4D(Vector4D&& V) = default;

  /**
  * Copy constructor.
  *
  */
  Vector4D(const Vector4D& V);

  /**
  * Copy constructor for vec3 to vec4.
  *
  */
  Vector4D(const Vector3D& V);

  /**
  * Copy constructor for vec3 to vec4 with W value.
  *
  */
  Vector4D(const Vector3D& V, float w);

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
  Vector4D(float x, float y, float z, float w);

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
  float
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
  distance(const Vector4D& otherVector) const;

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
  distanceSqr(const Vector4D& otherVector) const;

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
  equals(const Vector4D& otherVector) const;

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
  equals(const Vector4D& otherVector, float errorRange) const;

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
  operator|(const Vector4D& B) const;

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
  Vector4D
  operator^(const Vector4D& B) const;

  //Uncommented
  Vector4D&
  operator=(const Vector4D& A);

  //Uncommented
  Vector4D&
  operator=(const Vector3D& A);

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
  operator*(const float scalar) const;

  //Uncommented
  Vector4D&
  operator*=(const float scalar);

  //Uncommented
  Vector4D
  operator/(const float scalar) const;

  //Uncommented
  Vector4D&
  operator/=(const float scalar);

  //Uncommented
  bool
  operator==(const Vector4D& otherVector);

  //Uncommented
  bool
  operator!=(const Vector4D& otherVector);

  //Uncommented
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