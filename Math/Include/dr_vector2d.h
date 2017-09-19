#pragma once

#include <cmath>
#include <stdexcept>
#include <dr_prerequisites.h>

namespace driderSDK {

class DR_API_EXPORT Vector2D
{
 public:
  /**
  * Default constructor
  *
  * Values are initialized with 0.
  */

  Vector2D();

  /**
  * Move constructor
  *
  */

  Vector2D(Vector2D&& V) = default;

  /**
  * Copy constructor
  *
  */

  Vector2D(const Vector2D& V);

  /**
  * Initialize constructor with values.
  *
  * @param x
  *  The x value of the vector
  *
  * @param y
  *  The y value of the vector
  */

  Vector2D(Float32 x, Float32 y);

  /**
  * Default destructor
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

  Float32
  dot(const Vector2D& B) const;

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

  Vector2D
  normalize() const;

  //Uncommented

  Float32&
  operator[](SizeT index);

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
  operator*(const Float32 S) const;

  //Uncommented

  Vector2D&
  operator*=(const Float32 S);

  //Uncommented

  Vector2D
  operator/(const Float32 S) const;

  //Uncommented

  Vector2D&
  operator/=(const Float32 S);

  Float32 x, y;
};

}